#!/bin/bash

set -ex

source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

# NB="$(get_nbsb_kube_remote nb)"
ovn_nb="tcp:$MASTER1:6641,tcp:$MASTER2:6641,tcp:$MASTER3:6641"
export OVN_NB_DB="${ovn_nb}"

ROLE="$(get_self_role)"

if [ "$ROLE" == "master" ]; then
  exec /opt/ovn-go-kube/ovnkube -daemonset -k8s-cacert "$(get_ca_cert_path)" -k8s-token "$(get_token)" -k8s-apiserver "$(get_api_server)" -cluster-subnet "$(get_cluster_cidr)" -net-controller
else
  exit 1
fi
