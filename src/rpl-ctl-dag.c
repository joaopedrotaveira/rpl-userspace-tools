/*
 *	RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks
 *	Userspace Tools
 *
 *	Authors:
 *	Joao Pedro Taveira	<joao.silva@inov.pt>
 *  Sergey Lapin <slapin@ossfans.org>
 *  Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 *  Maxim Osipov <maxim.osipov@siemens.com>
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

#include <rpl_nl.h>
#include <libcommon.h>

#include "rpl-ctl.h"


/******************
 * LIST handling  *
 ******************/

static rpl_ctl_res_t list_dag_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 2) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	/* rpl_ctl list 2001:0b18:2000:8221::1 */
	if (cmd->argc == 2) {
		cmd->dodagid = cmd->argv[1];
		cmd->flags = NLM_F_REQUEST;
	} else {
		/* rpl_ctl list */
		cmd->dodagid = NULL;
		cmd->flags = NLM_F_REQUEST | NLM_F_DUMP;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t list_dag_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	/* List single interface */
	if (cmd->dodagid)
		NLA_PUT_STRING(msg, RPL_ATTR_DODAG_ID, cmd->dodagid);

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t list_dag_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	uint8_t instance_id;
	uint16_t ocp;
	char * dodagid;
	uint8_t version;
	uint8_t	grounded;
	uint8_t	is_root;
	uint8_t mop;
	uint8_t dtsn;
	uint16_t rank;
	uint8_t dao_sequence;
	uint8_t pcs;
	uint8_t min_hop_rank_inc;


	/* Check for mandatory attributes */
	if (!attrs[RPL_ATTR_INSTANCE_ID] ||
	    !attrs[RPL_ATTR_OCP] ||
	    !attrs[RPL_ATTR_DODAG_ID] ||
	    !attrs[RPL_ATTR_VERSION] ||
	    !attrs[RPL_ATTR_GROUNDED] ||
	    !attrs[RPL_ATTR_MOP] ||
	    !attrs[RPL_ATTR_DTSN] ||
		!attrs[RPL_ATTR_RANK] ||
		!attrs[RPL_ATTR_DAO_SEQUENCE] ||
		!attrs[RPL_ATTR_PCS] ||
		!attrs[RPL_ATTR_MIN_HOP_RANK_INCR] ||
		!attrs[RPL_ATTR_IS_ROOT])
		return RPL_CTL_STOP_ERR;

	/* Get attribute values from the message */
	instance_id = nla_get_u8(attrs[RPL_ATTR_INSTANCE_ID]);
	ocp = nla_get_u16(attrs[RPL_ATTR_OCP]);
	dodagid = nla_get_string(attrs[RPL_ATTR_DODAG_ID]);

	version = nla_get_u8(attrs[RPL_ATTR_VERSION]);
	grounded = nla_get_u8(attrs[RPL_ATTR_GROUNDED]);
	is_root = nla_get_u8(attrs[RPL_ATTR_IS_ROOT]);
	mop = nla_get_u8(attrs[RPL_ATTR_MOP]);
	dtsn = nla_get_u8(attrs[RPL_ATTR_DTSN]);
	rank = nla_get_u16(attrs[RPL_ATTR_RANK]);
	dao_sequence = nla_get_u8(attrs[RPL_ATTR_DAO_SEQUENCE]);
	pcs = nla_get_u8(attrs[RPL_ATTR_PCS]);
	min_hop_rank_inc = nla_get_u8(attrs[RPL_ATTR_MIN_HOP_RANK_INCR]);

	/* Display information about interface */
	printf("%s DODAG object\n", dodagid);
	printf("InstanceID: %d\n", instance_id);
	printf("OF: %d\n", ocp);
	printf("Version: %d\n", version);
	printf("DodagID: %s\n",dodagid);
	printf("Rank: %d\n", rank);
	printf("Grounded: %d\n", grounded);
	printf("MOP: %d\n", mop);
	//printf("Prf: %d\n", preference);
	printf("DTSN: %d\n", dtsn);
	printf("Root: %d\n", is_root);
	//printf("AutoGen: %d\n", auto_gen);
	//printf("Auth: %d\n", authenticated);
	printf("PCS: %d\n", pcs);
	printf("DAO Sequence: %d\n", dao_sequence);
	//printf("DIOIntDoubl: %d\n", DIOIntDoubl);
	//printf("DIOIntMin: %d\n", DIOIntMin);
	//printf("DIORedun: %d\n", DIORedun);
	//printf("MaxRankIncrease: %u\n", MaxRankIncrease);
	printf("MinHopRankIncrease: %u\n", min_hop_rank_inc);
	//printf("def_lifetime: %d\n", def_lifetime);
	//printf("lifetime_unit: %u\n", lifetime_unit);

	printf("\n");

	return (cmd->flags & NLM_F_MULTI) ? RPL_CTL_CONT_OK : RPL_CTL_STOP_OK;
}

static rpl_ctl_res_t list_dag_finish(struct rpl_ctl_cmd *cmd)
{
	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event list_dag_response_event[] = {
	{
		.call = list_dag_response,
		.nl = RPL_LIST_DAG,
	},
	{},
};

/******************
 *  ADD handling  *
 ******************/

//static rpl_ctl_res_t add_phy_parse(struct rpl_ctl_cmd *cmd)
//{
//	if (cmd->argc < 2 || cmd->argc > 4) {
//		printf("Incorrect number of arguments!\n");
//		return RPL_CTL_STOP_ERR;
//	}
//
//	cmd->phy = cmd->argv[1];
//	cmd->iface = cmd->argv[2]; /* Either iface name or NULL */
//
//	return RPL_CTL_CONT_OK;
//}
//
//static rpl_ctl_res_t add_phy_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
//{
////	unsigned char hwa[RPL_ADDR_LEN];
////
////	/* add single interface */
////	NLA_PUT_STRING(msg, RPL_ATTR_PHY_NAME, cmd->phy);
////	if (cmd->iface)
////		NLA_PUT_STRING(msg, RPL_ATTR_DEV_NAME, cmd->iface);
////
////	NLA_PUT_U8(msg, RPL_ATTR_DEV_TYPE, RPL_DEV_WPAN);
////
////	if (cmd->argc >= 4) {
////		int ret = parse_hw_addr(cmd->argv[3], hwa);
////		if (ret) {
////			printf("Bad coordinator address!\n");
////			return RPL_CTL_STOP_ERR;
////		}
////		NLA_PUT(msg, RPL_ATTR_HW_ADDR,
////			RPL_ADDR_LEN, hwa);
////	}
////
////	return RPL_CTL_CONT_OK;
////
////nla_put_failure:
//	return RPL_CTL_STOP_ERR;
//
//}
//
//static rpl_ctl_res_t add_phy_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
//{
//	if (!attrs[RPL_ATTR_DEV_NAME] ||
//	    !attrs[RPL_ATTR_PHY_NAME])
//		return RPL_CTL_STOP_ERR;
//
//	printf("Registered new device ('%s') on phy %s\n",
//			nla_get_string(attrs[RPL_ATTR_DEV_NAME]),
//			nla_get_string(attrs[RPL_ATTR_PHY_NAME]));
//
//	return RPL_CTL_STOP_OK;
//}
//
//static struct rpl_ctl_cmd_event add_phy_response_event[] = {
//	{
//		.call = add_phy_response,
//		.nl = RPL_ENABLE_IFACE,
//	},
//	{},
//};

/******************
 *  DEL DAG handling  *
 ******************/

static rpl_ctl_res_t del_dag_parse(struct rpl_ctl_cmd *cmd)
{
	switch (cmd->argc) {
	case 2:
		cmd->dodagid = cmd->argv[1];
		break;
	default:
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t del_dag_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	/* del single interface */
	if (cmd->dodagid)
		NLA_PUT_STRING(msg, RPL_ATTR_DODAG_ID, cmd->dodagid);

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t del_dag_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	if (!attrs[RPL_ATTR_DODAG_ID])
		return RPL_CTL_STOP_ERR;

	printf("Removed DAG ('%s')\n",
			nla_get_string(attrs[RPL_ATTR_DODAG_ID]));

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event del_dag_response_event[] = {
	{
		.call = del_dag_response,
		.nl = RPL_DEL_DAG,
	},
	{},
};

const struct rpl_ctl_module rpl_ctl_dag = {
	.name = "DAGs",
	.commands = {
	{
		.name		= "list",
		.usage		= "[dodagid]",
		.doc		= "List DAG(s).",
		.nl_cmd		= RPL_LIST_DAG,
		.parse		= list_dag_parse,
		.request	= list_dag_request,
		.response	= list_dag_response_event,
		.finish		= list_dag_finish,
	},
//	{
//		.name		= "add",
//		.usage		= "dodagid [instanceID [OCP]]",
//		.doc		= "Add new DAG to Instance with OCP.",
//		.nl_cmd		= RPL_ADD_DAG,
//		.parse		= add_dag_parse,
//		.request	= add_dag_request,
//		.response	= add_dag_response_event,
//	},
	{
		.name		= "del",
		.usage		= "dodagid",
		.doc		= "Delete the specified DAG.",
		.nl_cmd		= RPL_DEL_DAG,
		.parse		= del_dag_parse,
		.request	= del_dag_request,
		.response	= del_dag_response_event,
	},
	{}}
};

