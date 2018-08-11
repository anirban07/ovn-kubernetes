#!/bin/bash

set -xe

source "$(dirname "${BASH_SOURCE[0]}")/ovn-central-common.inc"

ovn_nb="tcp:$MASTER1:6641,tcp:$MASTER2:6641,tcp:$MASTER3:6641"
ovn_sb="tcp:$MASTER1:6642,tcp:$MASTER2:6642,tcp:$MASTER3:6642"

exec ovn-northd -vconsole:info "--ovnnb-db=$ovn_nb" "--ovnsb-db=$ovn_sb"
