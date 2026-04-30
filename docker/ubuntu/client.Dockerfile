FROM ubuntu:26.04
COPY srcml_*_ubuntu26.04_*.deb /tmp/
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends /tmp/srcml_*_ubuntu26.04_*.deb \
 && rm /tmp/srcml_*.deb \
 && rm -rf /var/lib/apt/lists/*
ENTRYPOINT ["/bin/bash"]
