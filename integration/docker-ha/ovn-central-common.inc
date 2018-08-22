#!/bin/bash

OVNNB_SOCK=/var/run/openvswitch/ovnnb_db.sock

source "$(dirname "${BASH_SOURCE[0]}")/common-api.bash"
source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

mkdir -p /var/lib/openvswitch >/dev/null 2>&1
MASTER_IP="$(get_node_internal_ip anirban-kube-1)"

get_nbsb_db() {
  echo "/var/lib/openvswitch/ovn${1}_db.db"
}

get_nbsb_sock() {
  echo "/var/run/openvswitch/ovn${1}_db.sock"
}

get_nbsb_unixctl() {
  echo "/var/run/openvswitch/ovn${1}_db.ctl"
}

get_nbsb_schema() {
  echo "/usr/share/openvswitch/ovn-${1}.ovsschema"
}


get_nbsb_remote_db_table() {
  case "$1" in
    "nb")
      echo "OVN_Northbound,NB_Global,connections"
      ;;
    "sb")
      echo "OVN_Southbound,SB_Global,connections"
      ;;
  esac
}
exec_db() {
  set -x
  /usr/share/openvswitch/scripts/ovn-ctl \
  --db-${1}-cluster-local-addr=$MASTER_IP \
  --db-${1}-file=/var/lib/openvswitch/ovn${1}_db.db \
  --ovn-${1}-logfile=/var/log/ovs-custom/ovsdb_"$1".log \
  start_${1}_ovsdb
  tail -f /dev/null
}

exec_db_peer() {
  set -x
  /usr/share/openvswitch/scripts/ovn-ctl \
    --db-${1}-cluster-remote-addr=$MASTER_IP \
    --db-${1}-cluster-local-addr="$(get_self_internal_ip)" \
    --db-${1}-file=/var/lib/openvswitch/ovn${1}_db.db \
    --ovn-${1}-logfile=/var/log/ovs-custom/ovsdb_"$1".log \
    start_${1}_ovsdb 
  tail -f /dev/null
}
