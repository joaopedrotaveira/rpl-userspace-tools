/*
 *  rpl_nl.h
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

#ifndef RPL_NL_H
#define RPL_NL_H

#define RPL_NL_NAME "RPL"
#define RPL_MCAST_DAG_NAME "dag"

enum {
	__RPL_ATTR_INVALID,

	RPL_ATTR_DEV_NAME,
	RPL_ATTR_DEV_INDEX,

	RPL_ATTR_PAGE,
	RPL_ATTR_DAG_PAGE_LIST,

	RPL_ATTR_PHY_NAME,
	RPL_ATTR_DEV_TYPE,

	__RPL_ATTR_MAX,
};

#define RPL_ATTR_MAX (__RPL_ATTR_MAX - 1)

extern struct nla_policy rpl_policy[];

/* commands */
/* REQ should be responded with CONF
 * and INDIC with RESP
 */
enum {
	__RPL_COMMAND_INVALID,

	RPL_LIST_DAG,
	RPL_ADD_DAG,
	RPL_DEL_DAG,

	RPL_LIST_IFACE,
	RPL_ENABLE_IFACE,
	RPL_DISABLE_IFACE,

	__RPL_CMD_MAX,
};

#define RPL_CMD_MAX (__RPL_CMD_MAX - 1)

#endif /* RPL_NL_H */
