/*
 *	RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks
 *	Linux RPL implementation
 *
 *	Authors:
 *  Sergey Lapin <slapin@ossfans.org>
 *  Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 *  Maxim Osipov <maxim.osipov@siemens.com>
 *	Joao Pedro Taveira	<joao.silva@inov.pt>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License.
 */

#include <netlink/netlink.h>
#include <netlink/attr.h>
#include <rpl_nl.h>

#define NLA_HW_ADDR NLA_U64

struct nla_policy rpl_policy[RPL_ATTR_MAX + 1] = {
	[RPL_ATTR_DEV_NAME] = { .type = NLA_STRING, },
	[RPL_ATTR_DEV_INDEX] = { .type = NLA_U32, },
	[RPL_ATTR_PHY_NAME] = { .type = NLA_STRING, },

//	[RPL_ATTR_STATUS] = { .type = NLA_U8, },
//	[RPL_ATTR_SHORT_ADDR] = { .type = NLA_U16, },
//	[RPL_ATTR_HW_ADDR] = { .type = NLA_HW_ADDR, },
//	[RPL_ATTR_PAN_ID] = { .type = NLA_U16, },
//	[RPL_ATTR_CHANNEL] = { .type = NLA_U8, },
	[RPL_ATTR_PAGE] = { .type = NLA_U8, },
//	[RPL_ATTR_COORD_SHORT_ADDR] = { .type = NLA_U16, },
//	[RPL_ATTR_COORD_HW_ADDR] = { .type = NLA_HW_ADDR, },
//	[RPL_ATTR_COORD_PAN_ID] = { .type = NLA_U16, },
//	[RPL_ATTR_SRC_SHORT_ADDR] = { .type = NLA_U16, },
//	[RPL_ATTR_SRC_HW_ADDR] = { .type = NLA_HW_ADDR, },
//	[RPL_ATTR_SRC_PAN_ID] = { .type = NLA_U16, },
//	[RPL_ATTR_DEST_SHORT_ADDR] = { .type = NLA_U16, },
//	[RPL_ATTR_DEST_HW_ADDR] = { .type = NLA_HW_ADDR, },
//	[RPL_ATTR_DEST_PAN_ID] = { .type = NLA_U16, },

//	[RPL_ATTR_CAPABILITY] = { .type = NLA_U8, },
//	[RPL_ATTR_REASON] = { .type = NLA_U8, },
//	[RPL_ATTR_SCAN_TYPE] = { .type = NLA_U8, },
//	[RPL_ATTR_CHANNELS] = { .type = NLA_U32, },
//	[RPL_ATTR_DURATION] = { .type = NLA_U8, },
//	[RPL_ATTR_ED_LIST] = { .len = 27 },
	[RPL_ATTR_DAG_PAGE_LIST] = { .minlen = 0, .maxlen = 32 * 4, },
};

