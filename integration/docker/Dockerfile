FROM vmware/photon:2.0

MAINTAINER Bo Gan <ganb@vmware.com>

USER root

ENV PYTHONDONTWRITEBYTECODE yes

RUN mkdir /var/log/ovs-custom

RUN rm -rf /var/log/ovs-custom/*

RUN mkdir /etc/depmod.d

RUN tdnf erase -y toybox

RUN tdnf install --refresh -y -q procps-ng tmux g++ libsasl2-dev libcurl4-openssl-dev pkg-config libssl-dev libcurl4-openssl-dev gcc libcap-ng-devel build-essential util-linux findutils openssl systemd binutils autoconf automake libtool sed make tar gdb vim PyYAML bindutils iptables ovn-common ovn-host ovn-central git jq iproute2 strace && tdnf clean all

ADD ovn-go.tar.gz /opt/ovn-go-kube/

COPY gdb-commands.txt gdb-script.txt entrypoint.bash install-cni.bash kubeapi.bash common-api.bash init-node.bash watcher.bash ovsdb.bash ovs-vswitchd.bash ovn-controller.bash ovs-common.inc ovnnb-db.bash ovnsb-db.bash ovnnb-db-peer.bash ovnsb-db-peer.bash ovn-northd.bash ovn-central-common.inc init-connection.bash /root/

WORKDIR /root

RUN git clone https://github.com/anirban07/ovs.git

WORKDIR ovs

RUN ./boot.sh

RUN ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc CFLAGS="-g"

RUN make

RUN make install

ENV PATH "$PATH:/usr/share/openvswitch/scripts"

WORKDIR /root

ENTRYPOINT /root/entrypoint.bash
