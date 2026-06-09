##
# docker-bake.hcl
#
# Defines how the srcml Linux distribution Docker images are built. It builds images to:
#
# * Provide a build environment
# * Create and package installers
# * Test the installers
#
# The default is to create all build environment docker images:
#
#   docker buildx bake
#
# To see the full list of available targets, including distribution-specific ones:
#
#   docker buildx bake --list targets
#
# To see the available variables:
#
#   docker buildx bake --list variables
#
# In addition to the default build environment, there are specific targets for the following:
#
# * `build` - Build srcML and create the package installer
# * `package` - Download the installer files to a host directory
# * `log`  - Download the test logs to a host directory
# * `image` - Create an image with only the installer files (no o.s.)

# Override using the environment variable SRCML_BAKE_SRC. E.g.,
#   SRCML_BAKE_SRC="."
#   SRCML_BAKE_SRC="srcml.tar.gz"
#   SRCML_BAKE_SRC="https://github.com/srcML/srcML.git"
#   SRCML_BAKE_SRC="https://github.com/srcML/srcML.git#develop"
variable "SRCML_BAKE_SRC" {
  description = "Location of the source code"
  default = "https://github.com/srcML/srcML.git#v1.1.0"
}

# Override using the environment variable SRCML_BAKE_PRESET_SUFFIX
# E.g., SRCML_BAKE_PRESET_SUFFIX="-fast"
variable "SRCML_BAKE_PRESET_SUFFIX" {
  description = "Suffix for the workflow preset"
  default = ""
}

# Override using the environment variable SRCML_BAKE_ARCHITECTURE
# E.g., SRCML_BAKE_ARCHITECTURE="linux/arm64"
variable "SRCML_BAKE_ARCHITECTURE" {
  description = "Architectures to build on"
  default = "linux/amd64,linux/arm64"
}

# Override using the environment variable SRCML_BAKE_DESTINATION_DIR
# E.g., SRCML_BAKE_DESTINATION_DIR="../dists"
variable "SRCML_BAKE_DESTINATION_DIR" {
  description = "Local directory for export of packages"
  default = "./dist_packages"
}

# Override using the environment variable SRCML_BAKE_CONTEXT_DIR
# E.g., SRCML_BAKE_CONTEXT_DIR="~/srcML/docker"
variable "SRCML_BAKE_CONTEXT_DIR" {
  description = "Directory of context files"
  default = "./docker"
}

# Override using the environment variable SRCML_BAKE_REGISTRY. E.g.,
#   SRCML_BAKE_REGISTRY=""         docker buildx bake # Docker Hub
#   SRCML_BAKE_REGISTRY="ghcr.io"  docker buildx bake # GitHub Container Registry
variable "SRCML_BAKE_REGISTRY" {
  description = "Registry domain for default build environments"
  default = ""
}

# Override using the environment variable SRCML_BAKE_PACKAGE_REGISTRY. E.g.,
#   SRCML_BAKE_PACKAGE_REGISTRY=""         docker buildx bake # Docker Hub
#   SRCML_BAKE_PACKAGE_REGISTRY="ghcr.io"  docker buildx bake # GitHub Container Registry
variable "SRCML_BAKE_PACKAGE_REGISTRY" {
  description = "Registry domain for the package targets, package and log"
  default = "ghcr.io"
}

# Placeholder to redefine in docker-bake.override.hcl
variable "SRCML_BAKE_SRCML_VERSION" {
  description = "srcML version to embed in image data"
  default = ""
}

# Placeholder to redefine in docker-bake.override.hcl
variable "SRCML_BAKE_CMAKE_VERSION" {
  description = "CMake version"
  default = ""
}

# Override using the environment variable SRCML_BAKE_CACHE
# E.g., SRCML_BAKE_CACHE=type=local,src=bake_cache,dest=bake_cache
# E.g., SRCML_BAKE_CACHE=""
variable "SRCML_BAKE_CACHE" {
  description = "Cache for build layers"
  default = ""
}

# Placeholder for distributions. See docker-bake.override.hcl for configured
# Linux distributions and distribution-specific groups and targets
variable "distributions" {
  default = [ { id = "ubuntu", version_id = "24.04", java_version_id = "22.04", name = "Ubuntu 24.04", workflow = "ubuntu", java = "latest" } ]
}

# Default is to only create the build environment
group "default" {
  targets = [ for item in distributions : item.id ]
}

# Context of the Dockerfile
function "context" {
  params = [id]
  result = "${SRCML_BAKE_CONTEXT_DIR}/${id}"
}

# Context of the Dockerfile
function "workflowPreset" {
  params = [dist]
  result = "ci-${dist.workflow}${SRCML_BAKE_PRESET_SUFFIX}"
}

# Base target for common settings
target "base" {
  platforms = split(",", SRCML_BAKE_ARCHITECTURE)
  labels = {
    "org.opencontainers.image.version"  = SRCML_BAKE_SRCML_VERSION
    "org.opencontainers.image.licenses" = "GPL-3.0-only"
  }
  pull = true
  cache-to = [ SRCML_BAKE_CACHE ]
  cache-from = [ SRCML_BAKE_CACHE ]
}

# Build images for all Linux distributions
target "default" {
  name = targetName(dist)
  description = "srcML build environment for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Build Environment"
    "org.opencontainers.image.description" = <<EOF
The srcML build environment for ${dist.name}.
It includes the dependencies for building, packaging, and testing srcML on this platform.
EOF
  }
  matrix = {
    dist = distributions
  }
  context = context(dist.id)
  args = {
    TAG          = dist.version_id
    JAVA_TAG     = dist.java_version_id
    CMAKE_VERSION = SRCML_BAKE_CMAKE_VERSION
    CMAKE_BINARY = try(dist.cmake, "")
    JAVA_VERSION = try(dist.java, "")
    OPENSUSE     = try(dist.opensuse, "")
  }
  tags     = concat([tagName(dist), tagNameAlias(dist)])
  inherits = ["base"]
  # cache-from = ["type=local,src=./.docker-build-cache/${targetName(dist)}"]
  # cache-to   = ["type=local,dest=./.docker-build-cache/${targetName(dist)},mode=max"]
}

# Packages for all distributions
# Output to host directory ${SRCML_BAKE_DESTINATION_DIR}
# Create image whose only contents are the package
target "build" {
  name = categoryTarget(dist, "build")
  description = "srcML build for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Build"
    "org.opencontainers.image.description" = <<EOF
The srcML build and package for ${dist.name}.
EOF
    "org.opencontainers.image.source" = SRCML_BAKE_SRC
  }
  push = true
  matrix = {
    dist = distributions
  }
  dockerfile-inline = <<EOF
FROM ${tagName(dist)} AS builder
WORKDIR /src
ADD --link ["${SRCML_BAKE_SRC}", "."]
# RUN cmake --workflow --preset ${workflowPreset(dist)}
RUN cmake --preset ${workflowPreset(dist)}
RUN cmake --build --preset ${workflowPreset(dist)}
FROM builder AS packager
RUN cpack --preset ${workflowPreset(dist)}
RUN cmake --build --preset ci-install-package-${dist.workflow}
EOF
  tags     = [categoryTagName(dist, "build")]
  # output   = ["type=docker"]
  inherits = ["base"]
}

# Packages for all distributions
# Output to host directory ${SRCML_BAKE_DESTINATION_DIR}
# Create image whose only contents are the package
target "package" {
  name = categoryTarget(dist, "package")
  description = "srcML package for ${dist.name}"
  contexts = {
    build = "target:${categoryTarget(dist, "build")}"
  }
  depends-on = [
    categoryTarget(dist, "build"),
  ]
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Package Files"
    "org.opencontainers.image.description" = <<EOF
The srcML package package for ${dist.name}.
EOF
  }
  matrix = {
    dist = distributions
  }
  dockerfile-inline = <<EOF
FROM scratch AS dist
COPY --from="build" \
  /src-build/dist/*.rpm \
  /src-build/dist/*.deb \
  /src-build/dist/*.tar.gz \
  /src-build/dist/*.bz2 \
  /
EOF
  tags     = [categoryTagName(dist, "package")]
  output   = ["type=local,dest=${SRCML_BAKE_DESTINATION_DIR}"]
  no-cache = true
  inherits = ["base"]
}

# Individual test targets
target "test-client" {
  name = categoryTarget(dist, "test-client")
  description = "srcML client test for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Client Test"
    "org.opencontainers.image.description" = <<EOF
The srcML client test for ${dist.name}.
EOF
  }
  matrix = {
    dist = distributions
  }
  depends_on = [categoryTarget(dist, "build")]
  contexts = {
    packager = "target:${categoryTarget(dist, "build")}"
  }
  dockerfile-inline = <<EOF
FROM packager AS test_client
RUN cmake --build --preset ci-test-client-${dist.workflow}
EOF
  tags     = [categoryTagName(dist, "test-client")]
  inherits = ["base"]
}

target "test-libsrcml" {
  name = categoryTarget(dist, "test-libsrcml")
  description = "srcML libsrcml test for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} libsrcml Test"
    "org.opencontainers.image.description" = <<EOF
The srcML libsrcml test for ${dist.name}.
EOF
  }
  matrix = {
    dist = distributions
  }
  depends_on = [categoryTarget(dist, "build")]
  contexts = {
    packager = "target:${categoryTarget(dist, "build")}"
  }
  dockerfile-inline = <<EOF
FROM packager AS test_libsrcml
RUN cmake --build --preset ci-test-libsrcml-${dist.workflow}
EOF
  tags     = [categoryTagName(dist, "test-libsrcml")]
  inherits = ["base"]
}

target "test-parser" {
  name = categoryTarget(dist, "test-parser")
  description = "srcML parser test for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Parser Test"
    "org.opencontainers.image.description" = <<EOF
The srcML parser test for ${dist.name}.
EOF
  }
  matrix = {
    dist = distributions
  }
  depends_on = [categoryTarget(dist, "build")]
  contexts = {
    packager = "target:${categoryTarget(dist, "build")}"
  }
  dockerfile-inline = <<EOF
FROM packager AS test_parser
RUN cmake --build --preset ci-test-parser-${dist.workflow}
EOF
  tags     = [categoryTagName(dist, "test-parser")]
  inherits = ["base"]
}

# Test logs for all distributions
# Output to host directory ${SRCML_BAKE_DESTINATION_DIR}
target "log" {
  name = categoryTarget(dist, "log")
  matrix = { dist = distributions }
  contexts = {
    # Reference specific stages from the build target
    test-client = "target:${categoryTarget(dist, "test-client")}"
    test-parser = "target:${categoryTarget(dist, "test-parser")}"
    test-libsrcml = "target:${categoryTarget(dist, "test-libsrcml")}"
  }
  depends-on = [
    categoryTarget(dist, "test-client"),
    categoryTarget(dist, "test-parser"),
    categoryTarget(dist, "test-libsrcml")
  ]
  dockerfile-inline = <<EOF
FROM scratch AS dist
COPY --from=test-client /src-build/dist/*.log /
COPY --from=test-parser /src-build/dist/*.log /
COPY --from=test-libsrcml /src-build/dist/*.log /
EOF
  tags = [categoryTagName(dist, "log")]
  output = ["type=local,dest=${SRCML_BAKE_DESTINATION_DIR}"]
  no-cache = true
  inherits = ["base"]
}

target "image" {
  name = categoryTarget(dist, "image")
  description = "srcML package image for ${dist.name}"
  labels = {
    "org.opencontainers.image.title" = "srcML ${dist.name} Package Image"
    "org.opencontainers.image.description" = <<EOF
The srcML package image for ${dist.name}.
EOF
  }
  matrix = {
    dist = distributions
  }
  contexts = {
    build = "target:${categoryTarget(dist, "build")}"
  }
  depends-on = [
    categoryTarget(dist, "build"),
  ]
  dockerfile-inline = <<EOF
FROM scratch AS dist
COPY --from="build" \
  /src-build/dist/*.rpm \
  /src-build/dist/*.deb \
  /src-build/dist/*.tar.gz \
  /src-build/dist/*.bz2 \
  /
EOF
  tags     = [categoryTagName(dist, "image")]
  # output   = ["type=registry"]
  inherits = ["base"]
}

# Target name
function "targetName" {
  params = [item]
  result = baseTargetName(item.id, item.version_id)
}

# Target name with a category
function "categoryTarget" {
  params = [item, category]
  result = baseTargetName(item.id, appendCategory(item.version_id, category))
}

# Target name utility
function "baseTargetName" {
  params = [id, version_id]
  result = format("%s_%s", id, sanitize(version_id))
}

# Tag name
function "tagName" {
  params = [item]
  result = baseTagName(SRCML_BAKE_REGISTRY, item.id, item.version_id)
}

# Tag name with a category
function "categoryTagName" {
  params = [item, category]
  result = baseTagName(SRCML_BAKE_PACKAGE_REGISTRY, appendCategory(item.id, category), item.version_id)
}

# Tag name with a category
function "baseTagName" {
  params = [registry, id, version_id]
  result = format("${registry != "" ? "${registry}/" : ""}srcml/%s:%s", id, version_id)
}

# Tag name
function "tagNameAlias" {
  params = [item]
  result = baseTagName(SRCML_BAKE_REGISTRY, item.id, try(item.tag, item.version_id))
}

# Add a category to the id
function "appendCategory" {
  params = [id, category]
  result = format("%s_%s", id, category)
}
