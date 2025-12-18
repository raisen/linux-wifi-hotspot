#!/bin/bash
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

VERSION="4.7.2"
PKGNAME="linux-wifi-hotspot"

echo -e "${GREEN}Building ${PKGNAME} RPM package${NC}"

# Check if we're in the fedora directory
if [ ! -f "linux-wifi-hotspot.spec" ]; then
    echo -e "${RED}Error: Please run this script from the fedora directory${NC}"
    exit 1
fi

# Set up RPM build tree
echo -e "${YELLOW}Setting up RPM build environment...${NC}"
rpmdev-setuptree

# Create source tarball
echo -e "${YELLOW}Creating source tarball...${NC}"
cd ../..
tar -czf ${PKGNAME}-${VERSION}.tar.gz linux-wifi-hotspot/ \
    --exclude=.git \
    --exclude=build \
    --exclude=.devcontainer \
    --exclude=debian \
    --exclude=fedora \
    --transform "s,^linux-wifi-hotspot,${PKGNAME}-${VERSION},"

mv ${PKGNAME}-${VERSION}.tar.gz ~/rpmbuild/SOURCES/

# Copy spec file
echo -e "${YELLOW}Copying spec file...${NC}"
cd linux-wifi-hotspot/fedora
cp linux-wifi-hotspot.spec ~/rpmbuild/SPECS/

# Build RPM
echo -e "${YELLOW}Building RPM...${NC}"
cd ~/rpmbuild/SPECS
rpmbuild -ba linux-wifi-hotspot.spec

# Copy RPM to current directory
echo -e "${YELLOW}Copying RPM package...${NC}"
cp ~/rpmbuild/RPMS/x86_64/${PKGNAME}-${VERSION}*.rpm ~/rpmbuild/SRPMS/${PKGNAME}-${VERSION}*.rpm . 2>/dev/null || true

echo -e "${GREEN}Build complete!${NC}"
echo -e "RPM packages are available in ~/rpmbuild/RPMS/x86_64/"
echo -e "\nTo install:"
echo -e "  ${YELLOW}sudo dnf install ~/rpmbuild/RPMS/x86_64/${PKGNAME}-${VERSION}*.rpm${NC}"
