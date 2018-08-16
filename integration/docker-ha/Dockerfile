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
                                 cyrus-sasl \
                                 findutils \
                                 g++ \
                                 gcc \
                                 gdb \
                                 git \
                                 iproute2 \ 
                                 iptables \
                                 jq \ 
                                 libcap-ng-devel \
                                 libcurl4-openssl-dev \
                                 libcurl4-openssl-dev \
                                 libsasl2-dev \
                                 libssl-dev \
                                 libtool \
                                 make \
                                 openldap \
                                 openssl \
                                 ovn-central \ 
                                 ovn-common \ 
                                 ovn-host \
                                 pkg-config \
                                 procps-ng \
                                 PyYAML \ 
                                 sed \
                                 strace \ 
                                 systemd \
                                 tar \
                                 tmux \
                                 util-linux \
                                 vim \
                                 && \ 
    tdnf clean all

ADD ovn-go.tar.gz /opt/ovn-go-kube/

COPY entrypoint.bash install-cni.bash kubeapi.bash common-api.bash init-node.bash ovsdb.bash ovs-vswitchd.bash ovn-controller.bash ovs-common.inc ovnnb-db.bash ovnsb-db.bash ovnnb-db-peer.bash ovnsb-db-peer.bash ovn-northd.bash ovn-central-common.inc init-connection.bash /root/

WORKDIR /root

RUN git clone --single-branch -b anir-develop https://github.com/prasoontelang/ovs.git

# ADD ovs-ldap.tar.gz ovs-ldap/

WORKDIR ovs

RUN ./boot.sh

RUN ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc CFLAGS="-g"

RUN make

RUN make install

ENV PATH "$PATH:/usr/share/openvswitch/scripts"

WORKDIR /root

ENTRYPOINT /root/entrypoint.bash
