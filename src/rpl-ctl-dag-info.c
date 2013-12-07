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
#include <getopt.h>
#include <rpl_nl.h>
#include <libcommon.h>

#include "rpl-ctl.h"

/*
 * Nodes List OPs
 */

static rpl_ctl_res_t list_node_parse(struct rpl_ctl_cmd *cmd)
{
	int c;
	optind=0;
	while (1) {
		c = getopt(cmd->argc, cmd->argv, "l");
		if (c == -1)
			break;

		switch(c) {
		case 'l':
			cmd->list_mode = 1;
			break;
		default:
			return RPL_CTL_STOP_ERR;
		}
	}
	if (optind > cmd->argc) {
		return RPL_CTL_STOP_ERR;
	}

	cmd->argc = cmd->argc - optind;
	cmd->argv = cmd->argv + optind;

	if (cmd->argc == 1) {
		cmd->dodagid = cmd->argv[0];
	} else {
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

static rpl_ctl_res_t list_node_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	uint8_t instance_id;
	uint8_t is_dodag_parent;
	uint8_t is_dao_parent;
	uint8_t is_preferred;
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
	    !attrs[RPL_ATTR_IS_PREFERRED] ||
	    !attrs[RPL_ATTR_DTSN] ||
		!attrs[RPL_ATTR_RANK]){
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

	is_dodag_parent = nla_get_u8(attrs[RPL_ATTR_IS_DODAG_PARENT]);
	is_dao_parent = nla_get_u8(attrs[RPL_ATTR_IS_DAO_PARENT]);
	is_preferred = nla_get_u8(attrs[RPL_ATTR_IS_PREFERRED]);

	dev_name = nla_get_string(attrs[RPL_ATTR_DEV_NAME]);

	if(cmd->list_mode){
		printf("%s\t%s%%%s\t%d\t%s\t%d\t%d\t%s\t%s\n",
				(is_dodag_parent)?"parent":"neighbor",node_addr_str,dev_name,
				instance_id,
				dodagid_str,
				rank,
				dtsn,
				(is_dao_parent)?"daoparent":"",
				(is_dodag_parent && is_preferred)?"preferred":"");
	} else {
		/* Display information about interface */
		printf("%s: %s%%%s\n",(is_dodag_parent)?"Parent":"Neighbor",node_addr_str,dev_name);
		printf("InstanceID: %d\n", instance_id);
		printf("DodagID: %s\n",dodagid_str);
		printf("Rank: %d\n", rank);
		printf("DTSN: %d\n", dtsn);
		printf("DAO Parent: %s\n", (is_dao_parent)?"Yes":"No");
		printf("Preferred: %s\n", (is_dodag_parent && is_preferred)?"Yes":"No");
		printf("\n");
	}
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

/*
 * List Downward routes
 */

static rpl_ctl_res_t list_downward_routes_parse(struct rpl_ctl_cmd *cmd)
{
	int c;
	optind=0;
	while (1) {
		c = getopt(cmd->argc, cmd->argv, "l");
		if (c == -1)
			break;

		switch(c) {
		case 'l':
			cmd->list_mode = 1;
			break;
		default:
			return RPL_CTL_STOP_ERR;
		}
	}
	if (optind > cmd->argc) {
		return RPL_CTL_STOP_ERR;
	}

	cmd->argc = cmd->argc - optind;
	cmd->argv = cmd->argv + optind;

	if (cmd->argc == 1) {
		cmd->dodagid = cmd->argv[0];
	} else {
		cmd->dodagid = NULL;
	}
	cmd->flags = NLM_F_REQUEST | NLM_F_DUMP;

	return RPL_CTL_CONT_OK;

//	if (cmd->argc > 3) {
//		printf("Incorrect number of arguments!\n");
//		return RPL_CTL_STOP_ERR;
//	}
//
//	if (cmd->argc == 3) {
//		cmd->dodagid = cmd->argv[1];
//	} else {
//		/* rpl_ctl list */
//		cmd->dodagid = NULL;
//	}
//	cmd->flags = NLM_F_REQUEST | NLM_F_DUMP;
//
//	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t list_downward_routes_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
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

static rpl_ctl_res_t list_downward_routes_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	uint8_t instance_id;
	struct in6_addr *dodagid;
	char dodagid_str[INET6_ADDRSTRLEN+1];

	struct in6_addr *prefix;
	char prefix_str[INET6_ADDRSTRLEN+1];
	uint8_t prefix_len;

	struct in6_addr *next_hop;
	char next_hop_str[INET6_ADDRSTRLEN+1];

	uint8_t one_hop;
	char *dev_name;

	/* Check for mandatory attributes */
	if (!attrs[RPL_ATTR_INSTANCE_ID] ||
	    !attrs[RPL_ATTR_DODAG_ID] ||
		!attrs[RPL_ATTR_PREFIX] ||
		!attrs[RPL_ATTR_PREFIX_LEN] ||
		!attrs[RPL_ATTR_NEXT_HOP] ||
	    !attrs[RPL_ATTR_DEV_NAME] ||
		!attrs[RPL_ATTR_ONE_HOP]){
		return RPL_CTL_STOP_ERR;
	}

	/* Get attribute values from the message */
	instance_id = nla_get_u8(attrs[RPL_ATTR_INSTANCE_ID]);

	dodagid = nla_data(attrs[RPL_ATTR_DODAG_ID]);
	inet_ntop(AF_INET6,dodagid,dodagid_str,INET6_ADDRSTRLEN);

	prefix = nla_data(attrs[RPL_ATTR_PREFIX]);
	inet_ntop(AF_INET6,prefix,prefix_str,INET6_ADDRSTRLEN);

	prefix_len = nla_get_u8(attrs[RPL_ATTR_PREFIX_LEN]);

	next_hop = nla_data(attrs[RPL_ATTR_NEXT_HOP]);
	inet_ntop(AF_INET6,next_hop,next_hop_str,INET6_ADDRSTRLEN);

	one_hop = nla_get_u8(attrs[RPL_ATTR_ONE_HOP]);

	dev_name = nla_get_string(attrs[RPL_ATTR_DEV_NAME]);

	if(cmd->list_mode){
		printf("%s/%d\t%d\t%s\t%s%%%s\t%s\n",
				prefix_str,prefix_len,
				instance_id,
				dodagid_str,
				next_hop_str,dev_name,
				(one_hop)?"onehop":"multihop");
	} else {
		/* Display information about interface */
		printf("Target: %s/%d\n",prefix_str,prefix_len);
		printf("InstanceID: %d\n", instance_id);
		printf("DodagID: %s\n",dodagid_str);
		printf("NextHop: %s%%%s\n", next_hop_str, dev_name);
		printf("OneHop: %s\n", (one_hop)?"Yes":"No");
		printf("\n");
	}

	return (cmd->flags & NLM_F_MULTI) ? RPL_CTL_CONT_OK : RPL_CTL_STOP_OK;
}

static rpl_ctl_res_t list_downward_routes_finish(struct rpl_ctl_cmd *cmd)
{
	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event list_downward_routes_response_event[] = {
	{
		.call = list_downward_routes_response,
		.nl = RPL_LIST_DOWNWARD_ROUTES,
	},
	{},
};

const struct rpl_ctl_module rpl_ctl_dag_info = {
	.name = "DAG Info",
	.commands = {
	{
		.name		= "list-parents",
		.usage		= "[-l] [dodagid]",
		.doc		= "List DAG Parents.",
		.nl_cmd		= RPL_LIST_PARENTS,
		.parse		= list_node_parse,
		.request	= list_node_request,
		.response	= list_parent_response_event,
		.finish		= list_node_finish,
	},
	{
		.name		= "list-neighbors",
		.usage		= "[-l] [dodagid]",
		.doc		= "List DAG Neighbors.",
		.nl_cmd		= RPL_LIST_NEIGHBORS,
		.parse		= list_node_parse,
		.request	= list_node_request,
		.response	= list_neighbors_response_event,
		.finish		= list_node_finish,
	},
	{
		.name		= "show-downward-routes",
		.usage		= "[-l] [dodagid]",
		.doc		= "Show DAG Downward Routes.",
		.nl_cmd		= RPL_LIST_DOWNWARD_ROUTES,
		.parse		= list_downward_routes_parse,
		.request	= list_downward_routes_request,
		.response	= list_downward_routes_response_event,
		.finish		= list_downward_routes_finish,
	},
	{}}
};

