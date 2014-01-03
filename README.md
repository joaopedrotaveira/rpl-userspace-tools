rpl-userspace-tools
=========

RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks for Linux

## rpl-userspace-tools
Userspace tools to setup, manage, list and check RPL configurations

To build it:
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

To use this tools, your kernel must support RPL:
```
# enable RPL router
$ sudo rpl-ctl enable eth0

# disable RPL router
$ sudo rpl-ctl disable eth0

# list enabled interfaces
$ sudo rpl-ctl listif

# list DODAGs
$ sudo rpl-ctl list

# others commands
$ sudo rpl-ctl list-parents
$ sudo rpl-ctl list-neighbors
$ sudo rpl-ctl list-downward-routes

$ sudo rpl-ctl global-repair
$ sudo rpl-ctl local-repair
```

man:
```
Usage: ./src/rpl-ctl [options] [command]
RPL Configuration tool for network interfaces

Options:
  -d, --debug[=N]                print netlink messages and other debug information
  -v, --version                  print version
  -h, --help                     print help

Command Options:
  -l                             list mode. one record per line

Common commands:
  help  [command]
     Print detailed help for a command.

  event  [iface]
     Monitor events from the kernel (^C to stop).


DAGs Configuration commands:
  list  instanceID [dodagid]
     List DAG(s).

  del  dodagid
     Delete the specified DAG.

  listif
     List RPL enabled/disabled network interface.

  enable  [iface]
     Enable RPL on network interface.

  disable  [iface]
     Disable RPL on network interface.


DAG Info commands:
  list-parents  [-l] [dodagid]
     List DAG Parents.

  list-neighbors  [-l] [dodagid]
     List DAG Neighbors.

  show-downward-routes  [-l] [dodagid]
     Show DAG Downward Routes.


DAG Management commands:
  global-repair  [dodagid]
     Increment DODAG version and trigger DODAG version update.

  local-repair  [dodagid]
     Trigger DIO timers.

  dao-update  [dodagid]
     Increment DTSN and trigger DAO updates.


Report bugs to joao.silva@inov.pt

Userspace tools for RPL homepage <http://github.com/joaopedrotaveira/rpl-userspace-tools>
```
