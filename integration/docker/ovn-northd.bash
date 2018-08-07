#!/bin/bash

set -xe

source "$(dirname "${BASH_SOURCE[0]}")/ovn-central-common.inc"

exec ovn-northd -vconsole:info "--ovnnb-db=$ovn_nb" "--ovnsb-db=$ovn_sb"
