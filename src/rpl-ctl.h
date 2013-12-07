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

#ifndef RPL_CTL_H
#define RPL_CTL_H

struct rpl_ctl_cmd;
struct nl_msg;
struct nlattr;
struct genlmsghdr;

typedef enum {
	RPL_CTL_CONT_OK,
	RPL_CTL_STOP_OK,
	RPL_CTL_STOP_ERR,
} rpl_ctl_res_t;

/*
 * rpl_ctl command event
 *
 * This assumes that 0 is invalid nl_resp (end of the list)
 */
struct rpl_ctl_cmd_event {
	rpl_ctl_res_t (*call)(struct rpl_ctl_cmd *cmd, struct genlmsghdr *ghdr,
			struct nlattr **attrs);
	int nl;
};

/* receive all event */
#define RPL_CTL_RESPONSE_ALL (-1)

/* rpl_ctl command descriptor */
struct rpl_ctl_cmd_desc {
	const char *name;	/* Name (as in command line) */
	const char *usage;	/* Arguments list */
	const char *doc;	/* One line command description */
	unsigned char nl_cmd;	/* NL command ID */

	/* Parse command line, fill in rpl_ctl_cmd struct. */
	/* You must set cmd->flags here! */
	rpl_ctl_res_t (*parse)(struct rpl_ctl_cmd *cmd);

	/* Prepare an outgoing netlink message */
	/* If request is not defined, we go to receive wo sending message */
	rpl_ctl_res_t (*request)(struct rpl_ctl_cmd *cmd, struct nl_msg *msg);

	/* Handle an incoming netlink message */
	struct rpl_ctl_cmd_event *response;

	/* Handle the end of multipart message */
	rpl_ctl_res_t (*finish)(struct rpl_ctl_cmd *cmd);
};

/* Parsed command results */
struct rpl_ctl_cmd {
	int argc;	/* number of arguments to the command */
	char **argv;	/* NULL-terminated arrays of arguments */

	const struct rpl_ctl_cmd_desc *desc;

	/* Fields below are prepared by parse function */
	int flags;	/* NL message flags */
	int list_mode; /* present results, one per line */
	char *iface;	/* Interface for a command */
	char *dodagid;	/* dodagid for a command */

	/* Filled before calling response */
	uint32_t seq;
};

struct rpl_ctl_module {
	const char *name;
	const struct rpl_ctl_cmd_desc commands[];
};

void rpl_ctl_help(const char *pname);
const struct rpl_ctl_cmd_desc *get_cmd(const char *name);

#endif /* RPL_CTL_H */
