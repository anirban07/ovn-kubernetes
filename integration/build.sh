#!/bin/bash

shopt -s nullglob
set -xe

#rsync -ac banirban@10.62.12.114:/Users/banirban/ovs-ldap /home/anirban/ovs-ldap-remote/

cd "$(dirname "$0")"
G="$(git rev-parse --show-toplevel)"
K="$G/integration/docker"
if [ "$1" = "ha" ]; then
    K="${K}-ha"
fi
O="/home/anirban/ovs-ldap"

if [ ! -e ${O}/boot.sh ]; then
    echo "ovs-ldap is not mounted on ${O}"
    exit 1
fi

# pushd "$G/go-controller"
cd "$G/go-controller"
make
cd -
# popd

tar Cczf "$G/go-controller/_output/go/bin" "$K/ovn-go.tar.gz" .

tar Cczf "${O}" "${K}/ovs-ldap.tar.gz" .

cd "$K"
docker build -t anirban07/photon-2.0-ovnkube:latest .
