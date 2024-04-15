# Docker

Docker images to build, package, and test srcML.

All commands are given in the root directory of the srcML repository. They do not work in the docker subdirectory nor in the build directory.

## Build srcML on Linux

The build for srcML includes testing for client, libsrcml, and the parser.

To build srcML on all supported Linux platforms:

```console
docker compose up
```

To build srcML on the latest Ubuntu:

```console
docker compose up ubuntu
```

To build srcML on Ubuntu 24.04

```console
docker compose up ubuntu_24.04
```

To build srcML on all Ubuntu platforms:

```console
docker compose up $(docker compose config --services | grep ubuntu)
```

# Package srcML for Linux

To package srcML:

```console
docker compose --env-file .env-package up
```

All packages are output to the docker volume `srcml_data`. You can view the contents of the volume and download individual packages in the Docker Desktop.

To view via docker:

```console
docker run -v srcml_dist:/dist alpine ls /dist
```

To download to the tar file srcML.tar.gz:

```console
docker run -v srcml_dist:/dist alpine tar czf - -C /dist . > srcML.tar.gz
```

## Maintain Docker Images

The following concerns all Docker images. To selectively do any of the following, see the variations above.

Docker images are automatically pulled when they are not in the local cache. If they are in the local cache, they are **not** automatically updated. To update docker images:

```console
docker compose pull
```

If the Dockerfiles change, a new one added or a previous one updated, the docker image needs to be rebuilt. To build all docker images (after Dockerfile change):


```console
docker compose build
```

To push all docker images to Docker Hub (if you have permission):

```console
docker compose push
```
