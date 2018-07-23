#!/bin/bash

shopt -s nullglob
set -xe

cd "$(dirname "$0")"
G="$(git rev-parse --show-toplevel)"
K="$G/integration/docker"

# pushd "$G/go-controller"
cd "$G/go-controller"
make
cd -
# popd

tar Cczf "$G/go-controller/_output/go/bin" "$K/ovn-go.tar.gz" .

cd "$K"
docker build -t vmware/photon-2.0-ovnkube:v1.1.0 .
