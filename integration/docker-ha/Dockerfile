FROM vmware/photon:2.0

MAINTAINER Bo Gan <ganb@vmware.com>

USER root

ENV PYTHONDONTWRITEBYTECODE yes

RUN mkdir /var/log/ovs-custom

RUN rm -rf /var/log/ovs-custom/*

RUN mkdir /etc/depmod.d

RUN tdnf erase -y toybox

RUN tdnf install --refresh -y -q autoconf \
                                 automake \
                                 bindutils \
                                 binutils \
                                 build-essential \
                                 createrepo_c-0.10.0 \
                                 cyrus-sasl \
                                 e2fsprogs-devel-1.43.4 \
                                 elfutils-0.169 \
                                 file \
                                 findutils \
                                 g++ \
                                 gcc \
                                 gdb \
                                 git \
                                 glibc-devel-2.26 \
                                 gzip-1.8 \
                                 iproute2 \ 
                                 iptables \
                                 jansson-devel-2.10 \
                                 jq \ 
                                 libcap-ng-devel \
                                 libcurl4-openssl-dev \
                                 libsasl2-dev \
                                 libssl-dev \
                                 libtool \
                                 lightwave-client-1.3.1 \
                                 linux-api-headers-4.9.74 \
                                 make \
                                 openldap \
                                 openssl-1.0.2n \
                                 openssl-devel-1.0.2n \
                                 ovn-central \ 
                                 ovn-common \ 
                                 ovn-host \
                                 pkg-config \
                                 procps-ng-3.3.12 \
                                 python-pip-9.0.1 \
                                 PyYAML \ 
                                 rpm-build-4.13.0.1 \
                                 rpm-devel-4.13.0.1 \
                                 rpm-4.13.0.1 \
                                 sed \
                                 shadow-4.2.1 \
                                 strace \ 
                                 systemd \
                                 tar \
                                 tmux \
                                 util-linux-devel-2.29.2 \
                                 vim \
                                 zlib-1.2.11 \
                                 zlib-devel-1.2.11 \
                                 && \ 
    tdnf clean all

ADD ovn-go.tar.gz /opt/ovn-go-kube/

COPY entrypoint.bash install-cni.bash kubeapi.bash common-api.bash init-node.bash ovsdb.bash ovs-vswitchd.bash ovn-controller.bash ovs-common.inc ovnnb-db.bash ovnsb-db.bash ovnnb-db-peer.bash ovnsb-db-peer.bash ovn-northd.bash ovn-central-common.inc init-connection.bash /root/

WORKDIR /root

# RUN git clone https://github.com/anirban07/ovs.git

# WORKDIR ovs

ADD ovs-ldap.tar.gz ovs-ldap/

WORKDIR ovs-ldap

RUN ./boot.sh

RUN ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc CFLAGS="-g"

RUN make

RUN make install

ENV PATH "$PATH:/usr/share/openvswitch/scripts"

WORKDIR /root

ENTRYPOINT /root/entrypoint.bash
