#!/bin/bash

set -xe

source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

mkdir -p /etc/openvswitch
DB=/etc/openvswitch/conf.db

[ -f $DB ] || ovsdb-tool create $DB

COUNTER=0
while true
do
    COUNTER=COUNTER+1
    echo ">>> Counter is $COUNTER"
    exec ovsdb-server $DB -vconsole:info "--remote=punix:$DBSOCK" --log-file=/var/log/ovs-custom/ovsdb-server.log
    sleep 1
done
