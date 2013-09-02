/*
 *	RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks
 *	Userspace Tools
 *
 *	Authors:
 *	Joao Pedro Taveira	<joao.silva@inov.pt>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#include <arpa/inet.h>
#include <rpl_nl.h>
#include <libcommon.h>

#include "rpl-ctl.h"

static rpl_ctl_res_t list_node_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 3) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	/* rpl_ctl list 2001:0b18:2000:8221::1 */
	if (cmd->argc == 3) {
		cmd->dodagid = cmd->argv[1];
	} else {
		/* rpl_ctl list */
		cmd->dodagid = NULL;
	}
	cmd->flags = NLM_F_REQUEST | NLM_F_DUMP;

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t list_node_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	struct in6_addr dodagid;

	//FIXME ADD INSTANCE ID TO REQUEST
	NLA_PUT_U8(msg,RPL_ATTR_INSTANCE_ID,0);

	/* List single DAG */
	if (cmd->dodagid){
		inet_pton(AF_INET6,cmd->dodagid,&dodagid);
		NLA_PUT(msg, RPL_ATTR_DODAG_ID, sizeof(struct in6_addr), &dodagid);
	}

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

//static rpl_ctl_res_t list_parent_header_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
//{
//	printf("Parents List\n");
//	return RPL_CTL_CONT_OK;
//}
//
//static rpl_ctl_res_t list_neighbors_header_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
//{
//	printf("Neighbors List\n");
//	return RPL_CTL_CONT_OK;
//}

static rpl_ctl_res_t list_node_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	uint8_t instance_id;
	uint8_t is_dodag_parent;
	uint8_t is_dao_parent;
	struct in6_addr *dodagid;
	char dodagid_str[INET6_ADDRSTRLEN+1];

	struct in6_addr *node_addr;
	char node_addr_str[INET6_ADDRSTRLEN+1];

	char *dev_name;
	uint8_t dtsn;
	uint16_t rank;

	/* Check for mandatory attributes */
	if (!attrs[RPL_ATTR_INSTANCE_ID] ||
	    !attrs[RPL_ATTR_DODAG_ID] ||
	    !attrs[RPL_ATTR_NODE_ADDR] ||
	    !attrs[RPL_ATTR_DEV_NAME] ||
	    !attrs[RPL_ATTR_IS_DODAG_PARENT] ||
	    !attrs[RPL_ATTR_IS_DAO_PARENT] ||
	    !attrs[RPL_ATTR_DTSN] ||
		!attrs[RPL_ATTR_RANK]){
		printf("UPS\n");
		return RPL_CTL_STOP_ERR;
	}

	/* Get attribute values from the message */
	instance_id = nla_get_u8(attrs[RPL_ATTR_INSTANCE_ID]);

	dodagid = nla_data(attrs[RPL_ATTR_DODAG_ID]);
	inet_ntop(AF_INET6,dodagid,dodagid_str,INET6_ADDRSTRLEN);

	node_addr = nla_data(attrs[RPL_ATTR_NODE_ADDR]);
	inet_ntop(AF_INET6,node_addr,node_addr_str,INET6_ADDRSTRLEN);

	dtsn = nla_get_u8(attrs[RPL_ATTR_DTSN]);
	rank = nla_get_u16(attrs[RPL_ATTR_RANK]);

	is_dodag_parent = nla_get_u16(attrs[RPL_ATTR_IS_DODAG_PARENT]);
	is_dao_parent = nla_get_u16(attrs[RPL_ATTR_IS_DAO_PARENT]);

	dev_name = nla_get_string(attrs[RPL_ATTR_DEV_NAME]);

	/* Display information about interface */
	printf("%s: %s@%s\n",(is_dodag_parent)?"Parent":"Neighbor",node_addr_str,dev_name);
	printf("InstanceID: %d\n", instance_id);
	printf("DodagID: %s\n",dodagid_str);
	printf("Rank: %d\n", rank);
	printf("DTSN: %d\n", dtsn);
	printf("DAO Parent: %s\n", (is_dao_parent)?"Yes":"No");
	printf("\n");

	return (cmd->flags & NLM_F_MULTI) ? RPL_CTL_CONT_OK : RPL_CTL_STOP_OK;
}

static rpl_ctl_res_t list_node_finish(struct rpl_ctl_cmd *cmd)
{
	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event list_parent_response_event[] = {
	{
		.call = list_node_response,
		.nl = RPL_LIST_PARENTS,
	},
	{},
};

static struct rpl_ctl_cmd_event list_neighbors_response_event[] = {
	{
		.call = list_node_response,
		.nl = RPL_LIST_NEIGHBORS,
	},
	{},
};

const struct rpl_ctl_module rpl_ctl_dag_info = {
	.name = "DAG Info",
	.commands = {
			{
				.name		= "list-parents",
				.usage		= "[dodagid]",
				.doc		= "List DAG Parents.",
				.nl_cmd		= RPL_LIST_PARENTS,
				.parse		= list_node_parse,
				.request	= list_node_request,
				.response	= list_parent_response_event,
				.finish		= list_node_finish,
			},
			{
				.name		= "list-neighbors",
				.usage		= "[dodagid]",
				.doc		= "List DAG Neighbors.",
				.nl_cmd		= RPL_LIST_NEIGHBORS,
				.parse		= list_node_parse,
				.request	= list_node_request,
				.response	= list_neighbors_response_event,
				.finish		= list_node_finish,
			},
			{
				.name		= "show-downward-routes",
				.usage		= "[dodagid]",
				.doc		= "Show DAG Downward Routes.",
				.nl_cmd		= RPL_DOWNWARD_ROUTES,
//				.parse		= list_downward_routes_parse,
//				.request	= list_downward_routes_request,
//				.response	= list_downward_routes_response_event,
//				.finish		= list_downward_routes_finish,
			},
	{}}
};

