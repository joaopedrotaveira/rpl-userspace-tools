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

/*
 * DAG Global Repair
 */

static rpl_ctl_res_t dag_global_repair_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 3) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	if (cmd->argc == 3) {
		cmd->dodagid = cmd->argv[1];
	} else {
		cmd->dodagid = NULL;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t dag_global_repair_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	struct in6_addr *dodagid;

	//FIXME ADD INSTANCE ID TO REQUEST
	NLA_PUT_U8(msg,RPL_ATTR_INSTANCE_ID,0);

	if (cmd->dodagid){
		inet_pton(AF_INET6,cmd->dodagid,&dodagid);
		NLA_PUT(msg, RPL_ATTR_DODAG_ID, sizeof(struct in6_addr), &dodagid);
	}

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t dag_global_repair_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	struct in6_addr *dodagid;
	char dodagid_str[INET6_ADDRSTRLEN+1];

	if (!attrs[RPL_ATTR_DODAG_ID])
		return RPL_CTL_STOP_ERR;

	//TODO get instanceID and new Version

	dodagid = nla_data(attrs[RPL_ATTR_DODAG_ID]);
	inet_ntop(AF_INET6,dodagid,dodagid_str,INET6_ADDRSTRLEN);

	printf("Global Repair triggered on DAG ('%s')\n",dodagid_str);

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event dag_global_repair_response_event[] = {
	{
		.call = dag_global_repair_response,
		.nl = RPL_GLOBAL_REPAIR,
	},
	{},
};

/*
 * DAG Local Repair
 */

static rpl_ctl_res_t dag_local_repair_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 3) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	if (cmd->argc == 3) {
		cmd->dodagid = cmd->argv[1];
	} else {
		cmd->dodagid = NULL;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t dag_local_repair_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	struct in6_addr *dodagid;

	//FIXME ADD INSTANCE ID TO REQUEST
	NLA_PUT_U8(msg,RPL_ATTR_INSTANCE_ID,0);

	if (cmd->dodagid){
		inet_pton(AF_INET6,cmd->dodagid,&dodagid);
		NLA_PUT(msg, RPL_ATTR_DODAG_ID, sizeof(struct in6_addr), &dodagid);
	}

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t dag_local_repair_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	struct in6_addr *dodagid;
	char dodagid_str[INET6_ADDRSTRLEN+1];

	if (!attrs[RPL_ATTR_DODAG_ID])
		return RPL_CTL_STOP_ERR;

	//TODO get instanceID and new Version

	dodagid = nla_data(attrs[RPL_ATTR_DODAG_ID]);
	inet_ntop(AF_INET6,dodagid,dodagid_str,INET6_ADDRSTRLEN);

	printf("Local Repair triggered on DAG ('%s')\n",dodagid_str);

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event dag_local_repair_response_event[] = {
	{
		.call = dag_local_repair_response,
		.nl = RPL_LOCAL_REPAIR,
	},
	{},
};

/*
 * DAO Update Trigger
 */

static rpl_ctl_res_t dag_dao_update_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 3) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	if (cmd->argc == 3) {
		cmd->dodagid = cmd->argv[1];
	} else {
		cmd->dodagid = NULL;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t dag_dao_update_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	struct in6_addr *dodagid;

	//FIXME ADD INSTANCE ID TO REQUEST
	NLA_PUT_U8(msg,RPL_ATTR_INSTANCE_ID,0);

	if (cmd->dodagid){
		inet_pton(AF_INET6,cmd->dodagid,&dodagid);
		NLA_PUT(msg, RPL_ATTR_DODAG_ID, sizeof(struct in6_addr), &dodagid);
	}

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t dag_dao_update_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	struct in6_addr *dodagid;
	char dodagid_str[INET6_ADDRSTRLEN+1];

	if (!attrs[RPL_ATTR_DODAG_ID])
		return RPL_CTL_STOP_ERR;

	dodagid = nla_data(attrs[RPL_ATTR_DODAG_ID]);
	inet_ntop(AF_INET6,dodagid,dodagid_str,INET6_ADDRSTRLEN);

	printf("DAO Update triggered on DAG ('%s')\n",dodagid_str);

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event dag_dao_update_response_event[] = {
	{
		.call = dag_dao_update_response,
		.nl = RPL_DAO_UPDATE,
	},
	{},
};

const struct rpl_ctl_module rpl_ctl_dag_mng = {
	.name = "DAG Management",
	.commands = {
			{
				.name		= "global-repair",
				.usage		= "[dodagid]",
				.doc		= "Increment DODAG version and trigger DODAG version update.",
				.nl_cmd		= RPL_GLOBAL_REPAIR,
				.parse		= dag_global_repair_parse,
				.request	= dag_global_repair_request,
				.response	= dag_global_repair_response_event,
			},
			{
				.name		= "local-repair",
				.usage		= "[dodagid]",
				.doc		= "Trigger DIO timers.",
				.nl_cmd		= RPL_LOCAL_REPAIR,
				.parse		= dag_local_repair_parse,
				.request	= dag_local_repair_request,
				.response	= dag_local_repair_response_event,
			},
			{
				.name		= "dao-update",
				.usage		= "[dodagid]",
				.doc		= "Increment DTSN and trigger DAO updates.",
				.nl_cmd		= RPL_DAO_UPDATE,
				.parse		= dag_dao_update_parse,
				.request	= dag_dao_update_request,
				.response	= dag_dao_update_response_event,
			},
	{}}
};

