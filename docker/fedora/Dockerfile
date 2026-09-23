ARG TAG=42
ARG JAVA_TAG=${TAG}
ARG JAVA_VERSION=latest

# Generate lightweight jre
FROM fedora:${JAVA_TAG} AS java
ARG JAVA_VERSION
RUN dnf install -y \
        binutils \
        java-${JAVA_VERSION}-openjdk-headless \
        java-${JAVA_VERSION}-openjdk-jmods

RUN jlink --compress=2 --strip-debug --add-modules=java.base --output /opt/java \
    && strip -p --strip-unneeded /opt/java/lib/server/*.so \
    && strip -p --strip-unneeded /opt/java/lib/*.so

FROM fedora:${TAG}
ARG TAG
LABEL org.srcml.email="srcmldev@gmail.com"
LABEL org.srcml.url="srcml.org"
LABEL org.srcml.distro="fedora"
LABEL org.srcml.osversion="${TAG}"

# Copy lightweight jre install
COPY --link --from=java /opt/java /opt/java
ENV JAVA_HOME=/opt/java

# Avoid prompts for timezone
ENV TZ=US/Michigan
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Update and install dependencies
RUN dnf upgrade -y \
    && dnf install -y \
    bzip2 \
    ccache \
    cpio \
    curl \
    gcc-c++ \
    git \
    libarchive-devel \
    libcurl-devel \
    libxcb-devel \
    libxml2-devel \
    libxslt-devel \
    make \
    man \
    ninja-build \
    openssl-devel \
    rpm-build \
    rpm-sign \
    tree \
    valgrind \
    zip \
    && dnf clean all

# CMake
ARG TARGETPLATFORM
ARG CMAKE_VERSION=4.0.0
RUN case "${TARGETPLATFORM}" in \
         "linux/amd64")  ARCHITECTURE="x86_64"   ;; \
         "linux/arm64")  ARCHITECTURE="aarch64"  ;; \
         *)  exit 1 ;; \
    esac; \
    curl --insecure -L "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${ARCHITECTURE}.tar.gz" | tar xz --strip-components=1 -C /usr/local

# Put ccache into the path
ENV PATH=/usr/lib/ccache:$PATH
RUN export PATH=/usr/lib/ccache/:$PATH
