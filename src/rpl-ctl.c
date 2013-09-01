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

#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <getopt.h>

#include <rpl_nl.h>
//#include <nl802154.h>
#include <libcommon.h>

#include "rpl-ctl.h"

static int rpl_ctl_cb_seq_check(struct nl_msg *msg, void *arg);
static int rpl_ctl_cb_valid(struct nl_msg *msg, void *arg);
static int rpl_ctl_cb_finish(struct nl_msg *msg, void *arg);

#ifdef HAVE_GETOPT_LONG
static const struct option rpl_ctl_long_opts[] = {
	{ "debug", optional_argument, NULL, 'd' },
	{ "version", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, 0, NULL, 0 },
};
#endif

/* Expected sequence number */
static int rpl_ctl_seq = 0;

/* Parsed options */
static int rpl_ctl_debug = 0;

#define dprintf(lvl, fmt...)			\
	do {					\
		if (rpl_ctl_debug >= lvl)		\
			printf(fmt);		\
	} while(0)


/* Exit from receive loop (set from receive callback) */
static int rpl_ctl_exit = 0;

extern const struct rpl_ctl_module rpl_ctl_common;
extern const struct rpl_ctl_module rpl_ctl_dag;

static const struct rpl_ctl_module *rpl_ctl_modules[] = {
	&rpl_ctl_common,
	&rpl_ctl_dag,
	NULL
};

const struct rpl_ctl_cmd_desc *get_cmd(const char *name)
{
	int i, j;

	for (i = 0; rpl_ctl_modules[i]; i++) {
		for (j = 0; rpl_ctl_modules[i]->commands[j].name; j++) {
			if (!strcmp(name, rpl_ctl_modules[i]->commands[j].name)) {
				return &rpl_ctl_modules[i]->commands[j];
			}
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{
	int c;
	int i;
	int family;
	int group;
	struct nl_sock *nl;
	struct nl_msg *msg;
	char *dummy = NULL;

	/* Currently processed command info */
	struct rpl_ctl_cmd cmd;


	/* Parse options */
	while (1) {
#ifdef HAVE_GETOPT_LONG
		int opt_idx = -1;
		c = getopt_long(argc, argv, "d::vh", rpl_ctl_long_opts, &opt_idx);
#else
		c = getopt(argc, argv, "d::vh");
#endif
		if (c == -1)
			break;

		switch(c) {
		case 'd':
			if (optarg) {
				i = strtol(optarg, &dummy, 10);
				if (*dummy == '\0')
					rpl_ctl_debug = nl_debug = i;
				else {
					fprintf(stderr, "Error: incorrect debug level: '%s'\n", optarg);
					exit(1);
				}
			} else
				rpl_ctl_debug = nl_debug = 1;
			break;
		case 'v':
			printf(	"rpl_ctl " VERSION "\n"
				"Copyright (C) 2008, 2009 by Siemens AG\n"
				"License GPLv2 GNU GPL version 2 <http://gnu.org/licenses/gpl.html>.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n"
				"\n"
				"Written by Dmitry Eremin-Solenikov, Sergey Lapin and Maxim Osipov\n");
			return 0;
		case 'h':
			rpl_ctl_help(argv[0]);
			return 0;
		default:
			rpl_ctl_help(argv[0]);
			return 1;
		}
	}
	if (optind >= argc) {
		rpl_ctl_help(argv[0]);
		return 1;
	}

	memset(&cmd, 0, sizeof(cmd));

	cmd.argc = argc - optind;
	cmd.argv = argv + optind;

	/* Parse command */
	cmd.desc = get_cmd(argv[optind]);
	if (!cmd.desc) {
		printf("Unknown command %s!\n", argv[optind]);
		return 1;
	}
	if (cmd.desc->parse) {
		i = cmd.desc->parse(&cmd);
		if (i == RPL_CTL_STOP_OK) {
			return 0;
		} else if (i == RPL_CTL_STOP_ERR) {
			printf("Command line parsing error!\n");
			return 1;
		}
	}

	/* Prepare NL command */
	nl = nl_socket_alloc();
	if (!nl) {
		nl_perror(NLE_NOMEM, "Could not allocate NL handle");
		return 1;
	}
	genl_connect(nl);
	family = genl_ctrl_resolve(nl, RPL_NL_NAME);
	group = nl_get_multicast_id(nl,
			RPL_NL_NAME, RPL_MCAST_DAG_NAME);
	if (group < 0) {
		fprintf(stderr, "Could not get multicast group ID: %s\n", strerror(-group));
		return 1;
	}
	nl_socket_add_membership(nl, group);
	rpl_ctl_seq = nl_socket_use_seq(nl) + 1;
	nl_socket_modify_cb(nl, NL_CB_VALID, NL_CB_CUSTOM,
		rpl_ctl_cb_valid, (void*)&cmd);
	nl_socket_modify_cb(nl, NL_CB_FINISH, NL_CB_CUSTOM,
		rpl_ctl_cb_finish, (void*)&cmd);
	nl_socket_modify_cb(nl, NL_CB_SEQ_CHECK, NL_CB_CUSTOM,
		rpl_ctl_cb_seq_check, (void*)&cmd);

	/* Send request, if necessary */
	if (cmd.desc->request) {
		msg = nlmsg_alloc();
		if (!msg) {
			nl_perror(NLE_NOMEM, "Could not allocate NL message!\n"); /* FIXME: err */
			return 1;
		}
		genlmsg_put(msg, NL_AUTO_PID, NL_AUTO_SEQ, family, 0,
			cmd.flags, cmd.desc->nl_cmd, 1);

		if (cmd.desc->request(&cmd, msg) != RPL_CTL_CONT_OK) {
			printf("Request processing error!\n");
			return 1;
		}

		dprintf(1, "nl_send_auto_complete\n");
		nl_send_auto_complete(nl, msg);
		cmd.seq = nlmsg_hdr(msg)->nlmsg_seq;

		dprintf(1, "nlmsg_free\n");
		nlmsg_free(msg);
	}

	/* Received message handling loop */
	while (rpl_ctl_exit == RPL_CTL_CONT_OK) {
		int err = nl_recvmsgs_default(nl);
		if (err != NLE_SUCCESS) {
			nl_perror(err, "Receive failed");
			return 1;
		}
	}
	nl_close(nl);

	if (rpl_ctl_exit == RPL_CTL_STOP_ERR)
		return 1;

	return 0;
}

void rpl_ctl_help(const char *pname)
{
	int i, j;

	printf("Usage: %s [options] [command]\n", pname);
	printf("Configure RPL for network interfaces\n\n");

	printf("Options:\n");
	printf("  -d, --debug[=N]                print netlink messages and other debug information\n");
	printf("  -v, --version                  print version\n");
	printf("  -h, --help                     print help\n");

	/* Print short help for available commands */
	for (i = 0; rpl_ctl_modules[i]; i++) {
		printf("\n%s commands:\n", rpl_ctl_modules[i]->name);
		for (j = 0; rpl_ctl_modules[i]->commands[j].name; j++) {
			printf("  %s  %s\n     %s\n\n", rpl_ctl_modules[i]->commands[j].name,
				rpl_ctl_modules[i]->commands[j].usage,
				rpl_ctl_modules[i]->commands[j].doc);
		}
	}

	printf("\n");
	printf("Report bugs to " PACKAGE_BUGREPORT "\n\n");
	printf(PACKAGE_NAME " homepage <" PACKAGE_URL ">\n");
}

/* Callback for sequence number check */
static int rpl_ctl_cb_seq_check(struct nl_msg *msg, void *arg)
{
	uint32_t seq;

	if(nlmsg_get_src(msg)->nl_groups)
		return NL_OK;

	seq = nlmsg_hdr(msg)->nlmsg_seq;
	if (seq == rpl_ctl_seq) {
		if (!(nlmsg_hdr(msg)->nlmsg_flags & NLM_F_MULTI))
			rpl_ctl_seq ++;
		return NL_OK;
	}
	printf("Sequence number mismatch (%i, %i)!", seq, rpl_ctl_seq);
	return NL_SKIP;
}

/* Callback for received valid messages */
static int rpl_ctl_cb_valid(struct nl_msg *msg, void *arg)
{
	struct nlmsghdr *nlh = nlmsg_hdr(msg);
	struct nlattr *attrs[RPL_ATTR_MAX+1];
        struct genlmsghdr *ghdr;
	struct rpl_ctl_cmd *cmd = arg;
	int i;

	/* Validate message and parse attributes */
	genlmsg_parse(nlh, 0, attrs, RPL_ATTR_MAX, rpl_policy);

        ghdr = nlmsg_data(nlh);

	dprintf(1, "Received command %d (%d) for interface\n",
			ghdr->cmd, ghdr->version);

	for (i = 0; cmd->desc->response[i].nl; i++)
		if (cmd->desc->response[i].nl == ghdr->cmd ||
		    cmd->desc->response[i].nl == RPL_CTL_RESPONSE_ALL) {
			rpl_ctl_exit = cmd->desc->response->call(cmd, ghdr, attrs);
	}

	return 0;
}

/* Callback for the end of the multipart message */
static int rpl_ctl_cb_finish(struct nl_msg *msg, void *arg)
{
	struct nlmsghdr *nlh = nlmsg_hdr(msg);
	struct rpl_ctl_cmd *cmd = arg;

	dprintf(1, "Received finish for interface\n");

	if (cmd->seq == nlh->nlmsg_seq) {
		if (cmd->desc->finish)
			rpl_ctl_exit = cmd->desc->finish(cmd);
		else
			rpl_ctl_exit = RPL_CTL_STOP_ERR;
	}

	return 0;
}

