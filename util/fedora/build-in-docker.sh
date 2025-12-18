#!/bin/bash
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

VERSION="4.7.2"
IMAGE_NAME="linux-wifi-hotspot-rpm"
CONTAINER_NAME="lwh-rpm-build-$$"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

echo -e "${GREEN}Building ${IMAGE_NAME} Docker image${NC}"

# Build Docker image
echo -e "${YELLOW}Building Docker image...${NC}"
docker build -t ${IMAGE_NAME} -f "${SCRIPT_DIR}/Dockerfile" "${REPO_ROOT}"

# Create and run container
echo -e "${YELLOW}Creating RPM package in container...${NC}"
CONTAINER_ID=$(docker create ${IMAGE_NAME})

# Copy RPM from container
echo -e "${YELLOW}Copying RPM packages...${NC}"
docker cp ${CONTAINER_ID}:/root/rpmbuild/RPMS/x86_64/ ./rpms/
docker cp ${CONTAINER_ID}:/root/rpmbuild/SRPMS/ ./srpms/

# Clean up container
echo -e "${YELLOW}Cleaning up...${NC}"
docker rm ${CONTAINER_ID}

echo -e "${GREEN}Build complete!${NC}"
echo -e "RPM packages are available in:"
echo -e "  ${YELLOW}./rpms/${NC} - Binary RPM packages"
echo -e "  ${YELLOW}./srpms/${NC} - Source RPM packages"
echo -e "\nTo install:"
echo -e "  ${YELLOW}sudo dnf install ./rpms/linux-wifi-hotspot-*.rpm${NC}"
