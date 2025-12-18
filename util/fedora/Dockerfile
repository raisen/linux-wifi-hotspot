FROM fedora:latest

ARG GIT_TAG=master
ARG GIT_URL=https://github.com/lakinduakash/linux-wifi-hotspot/archive/${GIT_TAG}.tar.gz

# Install build dependencies
RUN dnf update -y && \
    dnf install -y \
    # Build tools
    rpm-build \
    rpmdevtools \
    make \
    gcc \
    gcc-c++ \
    pkg-config \
    # Development libraries
    gtk3-devel \
    libpng-devel \
    qrencode-devel \
    glib2-devel \
    # Runtime dependencies
    hostapd \
    iw \
    iproute \
    util-linux \
    procps-ng \
    dnsmasq \
    iptables \
    # Additional tools
    tar \
    curl \
    which \
    && dnf clean all

# Set up RPM build environment
WORKDIR /root
RUN rpmdev-setuptree

# Download and extract source
RUN curl -L --output linux-wifi-hotspot.tar.gz --silent ${GIT_URL}
RUN tar -xzf linux-wifi-hotspot.tar.gz

# Prepare source for RPM build
WORKDIR /root/linux-wifi-hotspot-${GIT_TAG}
RUN tar -czf ~/rpmbuild/SOURCES/linux-wifi-hotspot-4.7.2.tar.gz \
    --transform 's,^,linux-wifi-hotspot-4.7.2/,' \
    --exclude=.git \
    --exclude=build \
    --exclude=.devcontainer .

# Copy spec file from top-level fedora directory (build context is repo root)
WORKDIR /root/rpmbuild/SPECS
COPY fedora/linux-wifi-hotspot.spec .

# Build RPM
RUN rpmbuild -ba linux-wifi-hotspot.spec

WORKDIR /root/rpmbuild

ENTRYPOINT ["/bin/bash"]
