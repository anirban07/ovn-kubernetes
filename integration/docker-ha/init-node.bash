#!/bin/bash

set -ex

source "$(dirname "${BASH_SOURCE[0]}")/ovs-common.inc"

# NB="$(get_nbsb_kube_remote nb)"

ovn_nb="tcp:$MASTER1:6641,tcp:$MASTER2:6641,tcp:$MASTER3:6641"
ovn_sb="tcp:$MASTER1:6642,tcp:$MASTER2:6642,tcp:$MASTER3:6642"
export OVN_NB_DB="${ovn_nb}"

ROLE="$(get_self_role)"

if [ "$ROLE" == "master" ]; then
  exec /opt/ovn-go-kube/ovnkube -net-controller -daemonset -init-gateways -gateway-localnet -init-master "$(get_self_name)" \
  -k8s-cacert "$(get_ca_cert_path)" -k8s-token "$(get_token)" \
  -k8s-apiserver "$(get_api_server)" -cluster-subnet "$(get_cluster_cidr)" \
  -service-cluster-ip-range "$(get_service_cidr)" \
  -nodeport \
  -nb-address "${ovn_nb}" -sb-address "${ovn_sb}"

elif [ "$ROLE" == "worker" ]; then
  /opt/ovn-go-kube/ovnkube -daemonset -init-gateways -init-node "$(get_self_name)" -k8s-cacert "$(get_ca_cert_path)" \
  -k8s-token "$(get_token)" -k8s-apiserver "$(get_api_server)" -cluster-subnet "$(get_cluster_cidr)" \
  -service-cluster-ip-range "$(get_service_cidr)" \
  -nb-address "${ovn_nb}" -sb-address "${ovn_sb}"
  #sleep forever
  tail -f /dev/null
elif [ "$ROLE" == "master-peer" ]; then
  #sleep forever
  tail -f /dev/null
else
  exit 1
fi
