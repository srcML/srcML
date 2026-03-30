FROM srcml/ubuntu:latest

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        git \
        python3 \
        python3-pip \
        xz-utils \
        zip \
        unzip \
        ninja-build \
        pkg-config \
    && rm -rf /var/lib/apt/lists/*

ENV EMSDK=/opt/emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git ${EMSDK} \
    && ${EMSDK}/emsdk install latest \
    && ${EMSDK}/emsdk activate latest

RUN echo "source \${EMSDK}/emsdk_env.sh" >> /etc/bash.bashrc

ENV VCPKG_ROOT=/opt/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh

RUN echo "export PATH=$VCPKG_ROOT:\$PATH" >> /etc/bash.bashrc

WORKDIR /workspaces/srcML