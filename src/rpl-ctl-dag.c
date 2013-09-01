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
//#include <nl802154.h>
#include <libcommon.h>

#include "rpl-ctl.h"


/******************
 * LIST handling  *
 ******************/

static rpl_ctl_res_t list_phy_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc > 2) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	/* rpl_ctl list wpan0 */
	if (cmd->argc == 2) {
		cmd->phy = cmd->argv[1];
		cmd->flags = NLM_F_REQUEST;
	} else {
		/* rpl_ctl list */
		cmd->phy = NULL;
		cmd->flags = NLM_F_REQUEST | NLM_F_DUMP;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t list_phy_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	/* List single interface */
	if (cmd->phy)
		NLA_PUT_STRING(msg, RPL_ATTR_PHY_NAME, cmd->phy);

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t list_phy_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
//	char * dev_name;
//	uint8_t chan;
//
//	/* Check for mandatory attributes */
//	if (!attrs[RPL_ATTR_PHY_NAME] ||
//	    !attrs[RPL_ATTR_CHANNEL] ||
//	    !attrs[RPL_ATTR_PAGE])
//		return RPL_CTL_STOP_ERR;
//
//	/* Get attribute values from the message */
//	dev_name = nla_get_string(attrs[RPL_ATTR_PHY_NAME]);
//
//	/* Display information about interface */
//	printf("%-10s IEEE 802.15.4 PHY object\n", dev_name);
//	printf("    page: %d  channel: ",
//			nla_get_u8(attrs[RPL_ATTR_PAGE]));
//	chan = nla_get_u8(attrs[RPL_ATTR_CHANNEL]);
//	if (chan == 255)
//		printf("n/a\n");
//	else
//		printf("%d\n", chan);
//
//	if (attrs[RPL_ATTR_CHANNEL_PAGE_LIST]) {
//		int len = nla_len(attrs[RPL_ATTR_CHANNEL_PAGE_LIST]);
//		int i, j;
//		uint32_t *data = nla_data(attrs[RPL_ATTR_CHANNEL_PAGE_LIST]);
//		if (len % 4 != 0) {
//			printf("    Error in PAGE LIST\n");
//			return RPL_CTL_STOP_ERR;
//		}
//
//		for (i = 0; i < len / 4; i++) {
//			printf("    channels on page %d:", data[i] >> 27);
//			for (j = 0; j < 27; j++) {
//				if (data[i] & (1 << j))
//					printf(" %d", j);
//			}
//
//			printf("\n");
//		}
//	}
//	printf("\n");
//
	return (cmd->flags & NLM_F_MULTI) ? RPL_CTL_CONT_OK : RPL_CTL_STOP_OK;
}

static rpl_ctl_res_t list_phy_finish(struct rpl_ctl_cmd *cmd)
{
	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event list_phy_response_event[] = {
	{
		.call = list_phy_response,
		.nl = RPL_LIST_IFACE,
	},
	{},
};

/******************
 *  ADD handling  *
 ******************/

static rpl_ctl_res_t add_phy_parse(struct rpl_ctl_cmd *cmd)
{
	if (cmd->argc < 2 || cmd->argc > 4) {
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	cmd->phy = cmd->argv[1];
	cmd->iface = cmd->argv[2]; /* Either iface name or NULL */

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t add_phy_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
//	unsigned char hwa[RPL_ADDR_LEN];
//
//	/* add single interface */
//	NLA_PUT_STRING(msg, RPL_ATTR_PHY_NAME, cmd->phy);
//	if (cmd->iface)
//		NLA_PUT_STRING(msg, RPL_ATTR_DEV_NAME, cmd->iface);
//
//	NLA_PUT_U8(msg, RPL_ATTR_DEV_TYPE, RPL_DEV_WPAN);
//
//	if (cmd->argc >= 4) {
//		int ret = parse_hw_addr(cmd->argv[3], hwa);
//		if (ret) {
//			printf("Bad coordinator address!\n");
//			return RPL_CTL_STOP_ERR;
//		}
//		NLA_PUT(msg, RPL_ATTR_HW_ADDR,
//			RPL_ADDR_LEN, hwa);
//	}
//
//	return RPL_CTL_CONT_OK;
//
//nla_put_failure:
	return RPL_CTL_STOP_ERR;

}

static rpl_ctl_res_t add_phy_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	if (!attrs[RPL_ATTR_DEV_NAME] ||
	    !attrs[RPL_ATTR_PHY_NAME])
		return RPL_CTL_STOP_ERR;

	printf("Registered new device ('%s') on phy %s\n",
			nla_get_string(attrs[RPL_ATTR_DEV_NAME]),
			nla_get_string(attrs[RPL_ATTR_PHY_NAME]));

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event add_phy_response_event[] = {
	{
		.call = add_phy_response,
		.nl = RPL_ENABLE_IFACE,
	},
	{},
};

/******************
 *  DEL handling  *
 ******************/

static rpl_ctl_res_t del_phy_parse(struct rpl_ctl_cmd *cmd)
{
	switch (cmd->argc) {
	case 2:
		cmd->iface = cmd->argv[1];
		break;
	case 3:
		cmd->phy = cmd->argv[1];
		cmd->iface = cmd->argv[2];
		break;
	default:
		printf("Incorrect number of arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t del_phy_request(struct rpl_ctl_cmd *cmd, struct nl_msg *msg)
{
	/* add single interface */
	NLA_PUT_STRING(msg, RPL_ATTR_DEV_NAME, cmd->iface);
	if (cmd->phy)
		NLA_PUT_STRING(msg, RPL_ATTR_PHY_NAME, cmd->phy);

	return RPL_CTL_CONT_OK;

nla_put_failure:
	return RPL_CTL_STOP_ERR;
}

static rpl_ctl_res_t del_phy_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	if (!attrs[RPL_ATTR_DEV_NAME] ||
	    !attrs[RPL_ATTR_PHY_NAME])
		return RPL_CTL_STOP_ERR;

	printf("Removed device ('%s') from phy %s\n",
			nla_get_string(attrs[RPL_ATTR_DEV_NAME]),
			nla_get_string(attrs[RPL_ATTR_PHY_NAME]));

	return RPL_CTL_STOP_OK;
}

static struct rpl_ctl_cmd_event del_phy_response_event[] = {
	{
		.call = del_phy_response,
		.nl = RPL_DISABLE_IFACE,
	},
	{},
};

const struct rpl_ctl_module rpl_ctl_dag = {
	.name = "PHY 802.15.4",
	.commands = {
	{
		.name		= "listphy",
		.usage		= "[phy]",
		.doc		= "List phys(s).",
		.nl_cmd		= RPL_LIST_IFACE,
		.parse		= list_phy_parse,
		.request	= list_phy_request,
		.response	= list_phy_response_event,
		.finish		= list_phy_finish,
	},
	{
		.name		= "add",
		.usage		= "phy [name [hwaddr]]",
		.doc		= "Add an WPAN interface attached to specified phy.",
		.nl_cmd		= RPL_ENABLE_IFACE,
		.parse		= add_phy_parse,
		.request	= add_phy_request,
		.response	= add_phy_response_event,
	},
	{
		.name		= "del",
		.usage		= "[phy] iface",
		.doc		= "Delete the specified interface.",
		.nl_cmd		= RPL_DISABLE_IFACE,
		.parse		= del_phy_parse,
		.request	= del_phy_request,
		.response	= del_phy_response_event,
	},
	{}}
};

