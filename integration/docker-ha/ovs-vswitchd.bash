#!/bin/bash

set -xe

source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

LOCAL_IP="$(get_self_internal_ip)"
ENCAP_TYPE=geneve

ovn_nb="tcp:$MASTER1:6641,tcp:$MASTER2:6641,tcp:$MASTER3:6641"
ovn_sb="tcp:$MASTER1:6642,tcp:$MASTER2:6642,tcp:$MASTER3:6642"

OVN_SB_DB="$ovn_sb"
OVN_NB_DB="$ovn_nb"

while [ ! -S $DBSOCK ]
do
  echo "ovs-vswitchd.bash: Waiting for local OVS db process to start.."
  sleep 1
done

ovs-vsctl --no-wait set Open_vSwitch . \
  external_ids:ovn-remote="$OVN_SB_DB" \
  external_ids:ovn-nb="$OVN_NB_DB" \
  external_ids:ovn-encap-ip="$LOCAL_IP" \
  external_ids:ovn-encap-type="$ENCAP_TYPE"

echo "ovs-vswitchd.bash: Starting ovs-vswitchd.."
exec ovs-vswitchd "unix:$DBSOCK" -vconsole:info --pidfile=/var/run/openvswitch/ovs-vswitchd.pid
