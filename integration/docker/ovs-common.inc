#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/common-api.bash"

DBSOCK=/var/run/openvswitch/db.sock
MASTER1="10.118.101.29"
MASTER2="10.118.101.190"
MASTER3="10.118.101.16"

# ovn_nb="tcp://$MASTER1:$(get_nbsb_remote_port nb),tcp://$MASTER2:$(get_nbsb_remote_port nb),tcp://$MASTER3:$(get_nbsb_remote_port nb)"
# ovn_sb="tcp://$MASTER1:$(get_nbsb_remote_port sb),tcp://$MASTER2:$(get_nbsb_remote_port sb),tcp://$MASTER3:$(get_nbsb_remote_port sb)"

ovn_nb="tcp://$MASTER1:6641,tcp://$MASTER2:6641,tcp://$MASTER3:6641"
ovn_sb="tcp://$MASTER1:6642,tcp://$MASTER2:6642,tcp://$MASTER3:6642"

get_nbsb_kube_nodes() {
  #convert to [{name: , port: }..] array
  kube_api_get /api/v1/nodes | jq ".items | map(select(.metadata.labels.\"ovnkube/ovn${1}\" != null)) | map( { name : .metadata.name, port : .metadata.labels.\"ovnkube/ovn${1}\" })"
}

get_nbsb_kube_remote() {
  local R
  R="$(get_nbsb_kube_nodes "$1")"
  [ "$( echo "$R" | jq '. | length')" == 1 ] || return 1
  local N
  local P
  N="$(echo "$R" | jq -er '.[0].name')"
  P="$(echo "$R" | jq -er '.[0].port')"
  local A
  A="$(get_node_internal_ip "$N")"
  echo "$A:$P"
}

add_nbsb_kube_label() {
  kube_api_put_node "$(get_self_name)" "{\"metadata\":{\"labels\":{\"ovnkube/ovn${1}\":\"$(get_nbsb_remote_port "$1")\"}}}" | jq ".metadata.labels"
}

del_nbsb_kube_label() {
  kube_api_put_node "$(get_self_name)" "{\"metadata\":{\"labels\":{\"ovnkube/ovn${1}\":null}}}" | jq ".metadata.labels"
}
