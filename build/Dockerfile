FROM fedora:latest
ADD ./build/install-dependencies.sh ./
ADD ./build/system-auth ./
RUN dnf -y install 'dnf-command(copr)' \
    && dnf -y install make automake gcc gcc-c++ kernel-devel cmake ccache \
    && dnf -y install gdb \
    && dnf -y install devscripts debhelper fakeroot file rpm-build \
    && /bin/bash -x ./install-dependencies.sh && dnf -y update && dnf clean all \
    && echo 'ALL ALL=(ALL:ALL) NOPASSWD: ALL' >> /etc/sudoers \
    && cp system-auth /etc/pam.d \
    && echo 'Defaults !requiretty' >> /etc/sudoers
CMD /bin/bash
