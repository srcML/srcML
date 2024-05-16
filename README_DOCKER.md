# Docker

Docker images to build, package, and test srcML on Linux. The Dockerfiles are in the directory `docker`. A Docker Compose file, `compose.yml`, is in the root directory and manages docker image management (build, push, pull) and execution on all the Linux distributions for packaging.

All commands are given in the root directory of the srcML repository. They do not work in the docker subdirectory nor in the build directory.

## Build and Package srcML on Linux

The build for srcML includes testing for the client, libsrcml, and the parser. By default, it runs the cmake workflow `ci-package-<installer>` where the <installer> is `deb` or `rpm`. I.e., for Ubuntu, it is equivalent to running the following:

```console
cmake --workflow --preset ci-package-deb
```

To build and package srcML on all supported Linux distributions:

```console
docker compose up
```

All packages are saved to the docker volume `srcml_dist`. The Docker Desktop allows you to view the volume's contents and download individual packages.

To build and package srcML on Ubuntu 24.04

```console
docker compose up ubuntu_24.04
```

To build and package srcML for all Ubuntu distributions:

```console
docker compose --profile ubuntu up
```

All of the Linux distributions are organized into Docker Compose profiles. They include a default profile, `platform`, Linux distribution families, `ubuntu`, `fedora`, and `opensuse`, and recentness, `latest`, `earliest`.

There are some special Docker Compose services to manage the built packages.

To view the packages via docker:

```console
docker compose run view
```

To combine all packages into one archive file:

```console
docker compose run tar
```

To remove all generated packages:

```console
docker compose run clean
```

## Maintain Docker Images

The following commands apply to all Docker images. To selectively do any of the following, see the variations above.

Docker images are automatically pulled when they are not in the local cache. If they are in the local cache, they are **not** automatically updated. To update docker images:

```console
docker compose pull
```

If the Dockerfiles change, a new one is added, or a previous one is updated, the Docker image needs to be rebuilt. To build all docker images (after Dockerfile change):

```console
docker compose build
```

To push all docker images to Docker Hub (if you have permission):

```console
docker compose push
```
