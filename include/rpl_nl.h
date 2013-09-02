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
	RPL_ATTR_DEV_ENABLED,
	RPL_ATTR_DEV_AUTOGEN,

	RPL_ATTR_OCP,
	RPL_ATTR_INSTANCE_ID,

	RPL_ATTR_DODAG_ID,
	RPL_ATTR_RANK,
	RPL_ATTR_VERSION,
	RPL_ATTR_MOP,
	RPL_ATTR_DTSN,
	RPL_ATTR_DAO_SEQUENCE,
	RPL_ATTR_GROUNDED,
	RPL_ATTR_PCS,

	RPL_ATTR_IS_ROOT,

	RPL_ATTR_DIO_INT_DOUBL,
	RPL_ATTR_DIO_INT_MIN,
	RPL_ATTR_DIO_REDUN,
	RPL_ATTR_MAX_RANK_INCR,
	RPL_ATTR_MIN_HOP_RANK_INCR,

	RPL_ATTR_DEF_LIFETIME,
	RPL_ATTR_LIFETIME_UNIT,

	RPL_ATTR_NODE_ADDR,
	RPL_ATTR_IS_DODAG_PARENT,
	RPL_ATTR_IS_DAO_PARENT,

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

	RPL_LIST_INSTANCE,

	RPL_LIST_OF,

	/* Management */
	RPL_GLOBAL_REPAIR,
	RPL_LOCAL_REPAIR,
	RPL_DAO_UPDATE,

	/* Information */
	RPL_LIST_PARENTS,
	RPL_LIST_NEIGHBORS,
	RPL_DOWNWARD_ROUTES,

	/* Configuration */
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
