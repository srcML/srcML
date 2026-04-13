##
# docker-bake.override.hcl
#
# The definition of specific Linux distributions (id) and versions (version_id) and setup for
# Linux distribution groups and targets. See docker-bake.hcl for the general command an primary
# groups.
#
# The main target groups are `default` (image for the srcML build environment) and `build` (build
# and package srcML). The rest of the groups are to extract the srcML installer packages: `image`,
# `package`, and `log`. See docker-bake.hcl for details.
#
# In addition to these targets for all distributions, there are targets to narrow the focus down to
# specific distributions, specific versions of those distributions, and specific target groups:
#
#   docker buildx bake ubuntu             - Create build environment image for all Ubuntu versions
#   docker buildx bake ubuntu_24_04       - Create build environment image for Ubuntu:24.04
#   docker buildx bake ubuntu_24_04_file  - Output package to local directory for Ubuntu:24.04
#
# Note the underscore, '_', instead of a dot, '.', as a separator for the version id, (i.e., "tag").

# All supported Linux distributions
# NOTE: Any distributions added or deleted must be reflected in the services in compose.yml
variable "distributions" {
  description = "Table of supported Linux distributions"
  default = [
    { id = "ubuntu",   version_id = "26.04",  java_version_id="22.04", name = "Ubuntu 26.04", workflow = "ubuntu", java = "latest", tag = "latest" },
    { id = "ubuntu",   version_id = "25.10",  java_version_id="22.04", name = "Ubuntu 25.10", workflow = "ubuntu", java = "latest" },
    { id = "ubuntu",   version_id = "25.04",  java_version_id="22.04", name = "Ubuntu 25.04", workflow = "ubuntu", java = "latest" },
    { id = "ubuntu",   version_id = "24.10",  java_version_id="22.04", name = "Ubuntu 24.10", workflow = "ubuntu", java = "latest" },
    { id = "ubuntu",   version_id = "24.04",  java_version_id="22.04", name = "Ubuntu 24.04", workflow = "ubuntu", java = "latest" },
    { id = "ubuntu",   version_id = "22.04",  java_version_id="22.04", name = "Ubuntu 22.04", workflow = "ubuntu" },
    { id = "ubuntu",   version_id = "20.04",  java_version_id="20.04", name = "Ubuntu 20.04", workflow = "ubuntu", cmake = "ON", tag = "earliest" },
    { id = "fedora",   version_id = "44",     java_version_id="38",    name = "Fedora 44", workflow = "rpm",    java = "latest", tag = "latest" },
    { id = "fedora",   version_id = "43",     java_version_id="38",    name = "Fedora 43", workflow = "rpm",    java = "latest" },
    { id = "fedora",   version_id = "42",     java_version_id="38",    name = "Fedora 42", workflow = "rpm",    java = "latest" },
    { id = "fedora",   version_id = "41",     java_version_id="38",    name = "Fedora 41", workflow = "rpm",    java = "17" },
    { id = "fedora",   version_id = "40",     java_version_id="38",    name = "Fedora 40", workflow = "rpm",    java = "17" },
    { id = "fedora",   version_id = "39",     java_version_id="38",    name = "Fedora 39", workflow = "rpm",    java = "17" },
    { id = "fedora",   version_id = "38",     java_version_id="38",    name = "Fedora 38", workflow = "rpm",    java = "17", tag = "earliest" },
    { id = "opensuse", version_id = "15.6",   java_version_id="15.6",  name = "OpenSUSE 15.6", workflow = "rpm",    opensuse="leap", tag = "latest" },
    { id = "opensuse", version_id = "15.5",   java_version_id="15.6",  name = "OpenSUSE 15.5", workflow = "rpm",    opensuse="leap" },
    { id = "opensuse", version_id = "15.4",   java_version_id="15.6",  name = "OpenSUSE 15.4", workflow = "rpm",    opensuse="leap" },
    { id = "opensuse", version_id = "15.3",   java_version_id="15.6",  name = "OpenSUSE 15.3", workflow = "rpm",    opensuse="leap" },
    { id = "opensuse", version_id = "15.2",   java_version_id="15.6",  name = "OpenSUSE 15.2", workflow = "rpm",    opensuse="leap" },
    { id = "opensuse", version_id = "15.1",   java_version_id="15.6",  name = "OpenSUSE 15.1", workflow = "rpm",    opensuse="leap", tag = "earliest" },
    { id = "opensuse", version_id = "15",     java_version_id="15.6",  name = "OpenSUSE 15", workflow = "rpm",    opensuse="leap" },
    { id = "opensuse", version_id = "latest", java_version_id="latest",  name = "OpenSUSE Tumbleweed", workflow = "rpm",    opensuse="tumbleweed", tag = "latest" }
  ]
}

# Override using the environment variable SRCML_BAKE_CMAKE_VERSION
# E.g., SRCML_BAKE_CMAKE_VERSION="3.31.6"
variable "SRCML_BAKE_CMAKE_VERSION" {
  description = "CMake version"
  default = "4.0.0"
}

# Override using the environment variable SRCML_BAKE_SRCML_VERSION
# E.g., SRCML_BAKE_SRCML_VERSION="1.1.0"
variable "SRCML_BAKE_SRCML_VERSION" {
  description = "srcML version to embed in image data"
  default = "1.1.0"
}

# Ubuntu build images
# Example target name: ubuntu_24_04 Example tag: srcml/ubuntu:24.04
group "ubuntu" { targets = distributionTarget("ubuntu") }

# Fedora build images
# Example target name: fedora_42 Example tag: srcml/fedora:42
group "fedora" { targets = distributionTarget("fedora") }

# OpenSUSE build images
# Example target name: opensuse_15_6 Example tag: srcml/opensuse:15.6
# Tumbleweed target name: opensuse_tumbleweed Tumbleweed tag: srcml/opensuse:tumbleweed
group "opensuse" { targets = distributionTarget("opensuse") }

# Convenience groups
group "rpm" {
  targets = [ for item in distributions : targetName(item) if item.workflow == "rpm"]
}
group "latest" {
  targets = [ for item in distributions : targetName(item) if try(item.tag, "") == "latest"]
}
group "earliest" {
  targets = [ for item in distributions : targetName(item) if try(item.tag, "") == "earliest"]
}

# Ubuntu build images
# Example target name: ubuntu_build_24_04 Example tag: srcml/ubuntu_build:24.04
group "ubuntu_build" { targets = categoryDistributionTarget("ubuntu", "build") }

# Ubuntu package image
# Example target name: ubuntu_24_04_package Example tag: srcml/ubuntu_image:24.04
group "ubuntu_image" { targets = categoryDistributionTarget("ubuntu", "image") }

# Ubuntu package package
# Example target name: ubuntu_24_04_package Example tag: srcml/ubuntu_package:24.04
group "ubuntu_package" { targets = categoryDistributionTarget("ubuntu", "package") }

# Ubuntu log images
# Example target name: ubuntu_24_04_log Example tag: srcml/ubuntu_log:24.04
group "ubuntu_log" { targets = categoryDistributionTarget("ubuntu", "log") }

# Fedora build images
# Example target name: fedora_build_42 Example tag: srcml/_buildfedora:42
group "fedora_build" { targets = categoryDistributionTarget("fedora", "build") }

# OpenSUSE build images
# Example target name: opensuse_build_15_6 Example tag: srcml/_buildopensuse:15.6
# Tumbleweed target name: opensuse_tumbleweed Tumbleweed tag: srcml/opensuse:tumbleweed
group "opensuse_build" { targets = categoryDistributionTarget("opensuse", "build") }

# Fedora image images
# Example target name: fedora_image_42 Example tag: srcml/_imagefedora:42
group "fedora_image" { targets = categoryDistributionTarget("fedora", "image") }

# OpenSUSE image images
# Example target name: opensuse_image_15_6 Example tag: srcml/_imageopensuse:15.6
# Tumbleweed target name: opensuse_tumbleweed Tumbleweed tag: srcml/opensuse:tumbleweed
group "opensuse_image" { targets = categoryDistributionTarget("opensuse", "image") }

# Fedora log images
# Example target name: fedora_log_42 Example tag: srcml/_logfedora:42
group "fedora_log" { targets = categoryDistributionTarget("fedora", "log") }

# OpenSUSE log images
# Example target name: opensuse_log_15_6 Example tag: srcml/_logopensuse:15.6
# Tumbleweed target name: opensuse_tumbleweed Tumbleweed tag: srcml/opensuse:tumbleweed
group "opensuse_log" { targets = categoryDistributionTarget("opensuse", "log") }

# Specific distribution with category
function "distributionTarget" {
  params = [id]
  result = [ for item in distributions : targetName(item) if item.id == id ]
}

# Specfic distribution with category
function "categoryDistributionTarget" {
  params = [id, category]
  result = [ for item in distributions : categoryTarget(item, category) if item.id == id]
}
