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
#   [ -f "$(get_nbsb_db "$1" )" ] || ovsdb-tool create "$(get_nbsb_db "$1")" "$(get_nbsb_schema "$1")"
#   add_nbsb_kube_label "$1"
#   get_nbsb_kube_remote "$1"
#   local R
#   trap "{ del_nbsb_kube_label "$1"; }" EXIT
#   ovsdb-server -vconsole:info \
# "--remote=punix:$(get_nbsb_sock "$1")" \
# "--remote=db:$(get_nbsb_remote_db_table "$1")" \
# "--unixctl=$(get_nbsb_unixctl "$1")" \
# "--log-file=/var/log/ovs-custom/ovsdb_"$1".log" \
# "$(get_nbsb_db "$1")" || R=$?
#   del_nbsb_kube_label "$1" || :
#   return $R
  set -x
  /usr/share/openvswitch/scripts/ovn-ctl \
  --db-${1}-cluster-local-addr=$MASTER_IP \
  --db-${1}-cluster-local-port="$(get_nbsb_remote_port ${1})" \
  --db-${1}-file=/var/lib/openvswitch/ovn${1}_db.db \
  --ovn-${1}-logfile=/var/log/ovs-custom/ovsdb_"$1".log \
  start_${1}_ovsdb
  tail -f /dev/null
}

exec_db_peer() {
#   [ -f "$(get_nbsb_db "$1" )" ] || ovsdb-tool create "$(get_nbsb_db "$1")" "$(get_nbsb_schema "$1")"
#   add_nbsb_kube_label "$1"
#   get_nbsb_kube_remote "$1"
#   local R
#   trap "{ del_nbsb_kube_label "$1"; }" EXIT
#   ovsdb-server -vconsole:info \
# "--remote=punix:$(get_nbsb_sock "$1")" \
# "--remote=db:$(get_nbsb_remote_db_table "$1")" \
# "--unixctl=$(get_nbsb_unixctl "$1")" \
# "--log-file=/var/log/ovs-custom/ovsdb_"$1".log" \
# "$(get_nbsb_db "$1")" || R=$?
#   del_nbsb_kube_label "$1" || :
  set -x
  /usr/share/openvswitch/scripts/ovn-ctl \
    --db-${1}-cluster-remote-addr=$MASTER_IP \
    --db-${1}-cluster-remote-port="$(get_nbsb_remote_port ${1})" \
    --db-${1}-cluster-local-addr="$(get_self_internal_ip)" \
    --db-${1}-cluster-local-port="$(get_nbsb_remote_port ${1})" \
    --db-${1}-file=/var/lib/openvswitch/ovn${1}_db.db \
    --ovn-${1}-logfile=/var/log/ovs-custom/ovsdb_"$1".log \
    start_${1}_ovsdb 
  tail -f /dev/null
}
