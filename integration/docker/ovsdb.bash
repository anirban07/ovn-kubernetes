#!/bin/bash

set -xe

source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

mkdir -p /etc/openvswitch
DB=/etc/openvswitch/conf.db

[ -f $DB ] || ovsdb-tool create $DB

exec tmux new-session -d -s my_session 'gdb --args ovsdb-server $DB -vconsole:info "--remote=punix:$DBSOCK" --log-file=/var/log/ovs-custom/ovsdb-server.log'
