ARG OPENSUSE=leap
ARG TAG=15
FROM opensuse/$OPENSUSE:${TAG} AS opensuse

# Generate lightweight jre
FROM opensuse AS java
RUN zypper --non-interactive install --no-recommends \
    binutils \
    java-11-openjdk-devel \
    java-11-openjdk-jmods

RUN jlink --compress=2 --strip-debug --add-modules=java.base --output /opt/java \
    && strip -p --strip-unneeded /opt/java/lib/server/*.so \
    && strip -p --strip-unneeded /opt/java/lib/*.so

FROM opensuse
LABEL org.srcml.email="srcmldev@gmail.com"
LABEL org.srcml.url="srcml.org"
LABEL org.srcml.distro="opensuse"
LABEL org.srcml.osversion="${TAG}"

# Copy lightweight jre install
COPY --from=java /opt/java /opt/java
ENV JAVA_HOME=/opt/java

# Avoid prompts for timezone
ENV TZ=US/Michigan
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Update and install dependencies
RUN zypper --non-interactive install --no-recommends \
    bzip2 \
    ccache \
    curl \
    gcc-c++ \
    git \
    gzip \
    libarchive-devel \
    libcurl-devel \
    libopenssl-devel \
    libxml2-devel \
    libxslt-devel \
    make \
    man \
    ninja \
    rpm-build \
    tar \
    tree \
    valgrind \
    which \
    zip \
    && zypper clean -a

# CMake
ARG TARGETPLATFORM
RUN case "${TARGETPLATFORM}" in \
         "linux/amd64")  ARCHITECTURE="x86_64"   ;; \
         "linux/arm64")  ARCHITECTURE="aarch64"  ;; \
         *)  exit 1 ;; \
    esac; \
    curl --insecure -L "https://github.com/Kitware/CMake/releases/download/v3.29.0/cmake-3.29.0-linux-${ARCHITECTURE}.tar.gz" | tar xz --strip-components=1 -C /usr/local

# Put ccache into the path
ENV PATH /usr/lib/ccache:$PATH
RUN export PATH=/usr/lib/ccache/:$PATH
