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

#include <arpa/inet.h>
#include <rpl_nl.h>
#include <libcommon.h>

#include "rpl-ctl.h"

const struct rpl_ctl_module rpl_ctl_dag_mng = {
	.name = "DAG Management",
	.commands = {
			{
				.name		= "global-repair",
				.usage		= "[dodagid]",
				.doc		= "Increment DODAG version and trigger DODAG version update.",
				.nl_cmd		= RPL_GLOBAL_REPAIR,
//				.parse		= list_dag_parse,
//				.request	= list_dag_request,
//				.response	= list_dag_response_event,
//				.finish		= list_dag_finish,
			},
			{
				.name		= "local-repair",
				.usage		= "[dodagid]",
				.doc		= "Trigger DIO timers.",
				.nl_cmd		= RPL_LOCAL_REPAIR,
//				.parse		= list_dag_parse,
//				.request	= list_dag_request,
//				.response	= list_dag_response_event,
//				.finish		= list_dag_finish,
			},
			{
				.name		= "dao-update",
				.usage		= "[dodagid]",
				.doc		= "Increment DTSN and trigger DAO updates.",
				.nl_cmd		= RPL_DAO_UPDATE,
//				.parse		= list_dag_parse,
//				.request	= list_dag_request,
//				.response	= list_dag_response_event,
//				.finish		= list_dag_finish,
			},
	{}}
};

