ARG TAG=22.04
FROM ubuntu:${TAG} AS ubuntu

# Generate lightweight jre
FROM ubuntu AS java
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -q && apt-get upgrade -q -y && apt-get install -q -y \
    binutils \
    openjdk-11-jdk-headless

RUN jlink --compress=2 --strip-debug --add-modules=java.base --output /opt/java \
    && strip -p --strip-unneeded /opt/java/lib/server/*.so \
    && strip -p --strip-unneeded /opt/java/lib/*.so

FROM ubuntu
LABEL org.srcml.email="srcmldev@gmail.com"
LABEL org.srcml.url="srcml.org"
LABEL org.srcml.distro="ubuntu"
LABEL org.srcml.osversion="${TAG}"

# Copy lightweight jre install
COPY --from=java /opt/java /opt/java
ENV JAVA_HOME=/opt/java

# Avoid prompts for timezone
ENV TZ=US/Michigan
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Update and install dependencies
RUN apt-get update && apt-get install --no-install-recommends -y \
    ca-certificates \
    ccache \
    cpio \
    curl \
    dpkg-dev \
    file \
    g++ \
    git \
    libarchive-dev \
    libcurl4-openssl-dev \
    libxml2-dev \
    libxml2-utils \
    libxslt1-dev \
    make \
    man \
    ninja-build \
    tree \
    valgrind \
    zip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install cmake
# Install direct binary for Ubuntu 20.04
ARG TAG
ENV TAG=$TAG
ARG CMAKE_BINARY
ARG TARGETPLATFORM
RUN if [ "$CMAKE_BINARY" = "ON" ]; then case "${TARGETPLATFORM}" in \
         "linux/amd64")  ARCHITECTURE="x86_64"   ;; \
         "linux/arm64")  ARCHITECTURE="aarch64"  ;; \
         *)  exit 1 ;; \
    esac; \
    curl --insecure -L "https://github.com/Kitware/CMake/releases/download/v3.29.0/cmake-3.29.0-linux-${ARCHITECTURE}.tar.gz" | tar xz --strip-components=1 -C /usr/local; \
    fi
# Install via apt.kitware.com package repo for all but Ubuntu 20.04
RUN if [ "$CMAKE_BINARY" != "ON" ]; then apt-get update && \
    apt-get install gpg wget --no-install-recommends -y && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null && \
    apt-get update && \
    rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
    apt-get install kitware-archive-keyring && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install cmake --no-install-recommends -y && \
    rm -rf /var/lib/apt/lists/*; \
    fi

# Allow man pages to be installed
RUN sed -i '/path-exclude=\/usr\/share\/man\/*/c\#path-exclude=\/usr\/share\/man\/*' /etc/dpkg/dpkg.cfg.d/excludes

# Put ccache into the path
ENV PATH /usr/lib/ccache:$PATH
RUN export PATH=/usr/lib/ccache/:$PATH
