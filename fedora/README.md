# Building RPM for Fedora

This directory contains files for building an RPM package for Fedora.

## Prerequisites

Install required build tools:
```bash
sudo dnf install -y rpm-build rpmdevtools
```

## Build Instructions

### 1. Set up RPM build environment

```bash
rpmdev-setuptree
```

This creates the directory structure in `~/rpmbuild/`

### 2. Create source tarball

From the project root:
```bash
cd ..
tar -czf linux-wifi-hotspot-4.7.2.tar.gz linux-wifi-hotspot/ \
    --exclude=.git \
    --exclude=build \
    --exclude=.devcontainer \
    --exclude=debian \
    --exclude=fedora
mv linux-wifi-hotspot-4.7.2.tar.gz ~/rpmbuild/SOURCES/
```

### 3. Copy spec file

```bash
cp fedora/linux-wifi-hotspot.spec ~/rpmbuild/SPECS/
```

### 4. Build the RPM

```bash
cd ~/rpmbuild/SPECS
rpmbuild -ba linux-wifi-hotspot.spec
```

### 5. Install the RPM

```bash
sudo dnf install ~/rpmbuild/RPMS/x86_64/linux-wifi-hotspot-*.rpm
```

## What the package does

The RPM package automatically:
- Installs all binary files and resources
- Configures firewalld to allow DHCP (required for hotspot functionality)
- Sets SELinux to permissive mode for dnsmasq
- Cleans up these changes on uninstall

## Quick Build Script

You can also use the provided build script:

```bash
cd fedora
./build-rpm.sh
```

This will build and create the RPM in the current directory.
