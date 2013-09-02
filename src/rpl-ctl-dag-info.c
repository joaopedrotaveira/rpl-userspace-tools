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

const struct rpl_ctl_module rpl_ctl_dag_info = {
	.name = "DAG Info",
	.commands = {
			{
				.name		= "list-parent",
				.usage		= "[dodagid]",
				.doc		= "List DAG Parents.",
				.nl_cmd		= RPL_LIST_PARENT,
		//		.parse		= list_parent_parse,
		//		.request	= list_parent_request,
		//		.response	= list_parent_response_event,
		//		.finish		= list_parent_finish,
			},
			{
				.name		= "list-neighbors",
				.usage		= "[dodagid]",
				.doc		= "List DAG Neighbors.",
				.nl_cmd		= RPL_LIST_NEIGHBORS,
		//		.parse		= list_parent_parse,
		//		.request	= list_parent_request,
		//		.response	= list_parent_response_event,
		//		.finish		= list_parent_finish,
			},
			{
				.name		= "show-upward-routes",
				.usage		= "[dodagid]",
				.doc		= "Show DAG Upward Routes.",
				.nl_cmd		= RPL_UPWARD_ROUTES,
		//		.parse		= list_parent_parse,
		//		.request	= list_parent_request,
		//		.response	= list_parent_response_event,
		//		.finish		= list_parent_finish,
			},
			{
				.name		= "show-downward-routes",
				.usage		= "[dodagid]",
				.doc		= "Show DAG Downward Routes.",
				.nl_cmd		= RPL_DOWNWARD_ROUTES,
		//		.parse		= list_parent_parse,
		//		.request	= list_parent_request,
		//		.response	= list_parent_response_event,
		//		.finish		= list_parent_finish,
			},
	{}}
};

