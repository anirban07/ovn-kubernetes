#!/bin/bash

set -ex

source "$(dirname "${BASH_SOURCE[0]}")/ovn-central-common.inc"


ROLE="$(get_self_role)"

if [ "$ROLE" == "master" ] || [ "$ROLE" == "master-peer" ]; then
  ovn-nbctl set-connection ptcp:$(get_nbsb_remote_port nb)
  ovn-sbctl set-connection ptcp:$(get_nbsb_remote_port sb)
  ovn-sbctl set connection . inactivity_probe=600000
  tail -f /dev/null
else
  exit 1
fi
