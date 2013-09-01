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

/******************/
/* HELP handling  */
/******************/

static rpl_ctl_res_t help_parse(struct rpl_ctl_cmd *cmd)
{
	const struct rpl_ctl_cmd_desc *desc;

	if (cmd->argc > 2) {
		printf("Too many arguments!\n");
		return RPL_CTL_STOP_ERR;
	} else if (cmd->argc == 1) {
		rpl_ctl_help("rpl_ctl");
		return RPL_CTL_STOP_OK;
	}

	/* Search for command help */
	desc = get_cmd(cmd->argv[1]);
	if (!desc) {
		printf("Unknown command %s!\n", cmd->argv[1]);
		return RPL_CTL_STOP_ERR;
	} else {
		printf("%s %s\n\t%s\n\n", desc->name,
			desc->usage,
			desc->doc);
	}
	return RPL_CTL_STOP_OK;
}

/******************/
/* EVENT handling */
/******************/

static rpl_ctl_res_t event_parse(struct rpl_ctl_cmd *cmd)
{
	cmd->flags = 0;

	if (cmd->argc > 2) {
		printf("Too many arguments!\n");
		return RPL_CTL_STOP_ERR;
	}

	return RPL_CTL_CONT_OK;
}

static char *rpl_ctl_cmd_names[__RPL_CMD_MAX + 1] = {
	[0 ... __RPL_CMD_MAX] = "?? unknown ??",
	[__RPL_COMMAND_INVALID] = "RPL_COMMAND_INVALID",

	[RPL_LIST_DAG] = "RPL_LIST_DAG",
	[RPL_ADD_DAG] = "RPL_ADD_DAG",
	[RPL_DEL_DAG] = "RPL_DEL_DAG",

	[RPL_ENABLE_IFACE] = "RPL_ENABLE_IFACE",
	[RPL_DISABLE_IFACE] = "RPL_DISABLE_IFACE",
	[RPL_LIST_IFACE] = "RPL_LIST_IFACE",
};


static rpl_ctl_res_t event_response(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr, struct nlattr **attrs)
{
	const char *iface = nla_get_string(attrs[RPL_ATTR_DEV_NAME]);

	if (cmd->argv[1] && (!iface || strcmp(cmd->argv[1], iface)))
		return RPL_CTL_CONT_OK;

	if (!iface)
		iface = "?????";

	if (ghdr->cmd < __RPL_CMD_MAX)
		fprintf(stdout, "%s: %s (%i)\n", iface, rpl_ctl_cmd_names[ghdr->cmd], ghdr->cmd);
	else
		fprintf(stdout, "%s: UNKNOWN (%i)\n", iface, ghdr->cmd);

	fflush(stdout);

	return RPL_CTL_CONT_OK;
}

static rpl_ctl_res_t event_finish(struct rpl_ctl_cmd *cmd)
{
	return RPL_CTL_CONT_OK;
}

static struct rpl_ctl_cmd_event monitor_response_event[] = {
	{
		.call = event_response,
		.nl = RPL_CTL_RESPONSE_ALL,
	},
	{},
};

/* Command descriptors */
const struct rpl_ctl_module rpl_ctl_common = {
	.name = "Common",
	.commands = {
	{
		.name		= "help",
		.usage		= "[command]",
		.doc		= "Print detailed help for a command.",
		.parse		= help_parse,
	},
	{
		.name		= "event",
		.usage		= "[iface]",
		.doc		= "Monitor events from the kernel (^C to stop).",
		.parse		= event_parse,
		.response	= monitor_response_event,
		.finish		= event_finish,
	},
	{}}
};

