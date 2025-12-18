# Fedora Build Docker Image

The Dockerfile in this directory can be used to build the RPM package for this project.

## Building the RPM

1. Build the Docker image:
   ```bash
   docker build . -t linux-wifi-hotspot-rpm
   ```

2. Run the container:
   ```bash
   docker run -it linux-wifi-hotspot-rpm
   ```

3. All build artifacts will be located in `/root/rpmbuild`:
   - **RPMS/x86_64/** - Binary RPM packages
   - **SRPMS/** - Source RPM packages

4. Copy the RPM out of the container:
   ```bash
   # Get container ID
   docker ps -a
   
   # Copy RPM from container
   docker cp <container-id>:/root/rpmbuild/RPMS/x86_64/linux-wifi-hotspot-4.7.2-1.fc*.x86_64.rpm .
   ```

## Quick Build Script

Alternatively, use the provided build script:

```bash
./build-in-docker.sh
```

This will automatically build the image, create the RPM, and copy it to the current directory.

## Installing the RPM

Once you have the RPM file:

```bash
sudo dnf install ./linux-wifi-hotspot-*.rpm
```

## Building from a specific branch/tag

To build from a specific Git tag or branch:

```bash
docker build --build-arg GIT_TAG=v4.7.2 . -t linux-wifi-hotspot-rpm
```
