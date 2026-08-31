# Install check for Ubuntu
#
# Installs the generated srcML package into a stock Ubuntu base image (with no
# srcML build dependencies) to verify that the package's declared runtime
# dependencies resolve from the standard repositories, then smoke-tests the
# installed srcml. The package files come from the `build` named build context.

ARG TAG=22.04
FROM ubuntu:${TAG}

# capture the check outputs to a log file (also printed at the end of the build)
ENV INSTALLCHECK_LOG=/var/log/srcml-installcheck.log

# generated package files from the build stage
COPY --from=build /src-build/dist/ /packages/

# install the client package (not the -dev package); apt resolves dependencies
RUN apt-get update \
    && apt-get install --no-install-recommends -y /packages/srcml_*.deb \
    && rm -rf /var/lib/apt/lists/*

# smoke-test the installed client; assert the exact srcML for a known input
# (an unresolved runtime dependency can yield an empty unit but exit 0)
RUN srcml --help
RUN out="$(srcml --text='int n;' -l C++)" \
    && exp="$(printf '%s\n%s' \
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>' \
        '<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0" language="C++"><decl_stmt><decl><type><name>int</name></type> <name>n</name></decl>;</decl_stmt></unit>')" \
    && { echo '=== srcml --version ==='; srcml --version; \
         echo '=== srcml --text (source to srcML) ==='; printf '%s\n' "$out"; } >> "$INSTALLCHECK_LOG" \
    && [ "$out" = "$exp" ] \
    || { printf 'unexpected srcml output:\n%s\n' "$out"; exit 1; }

# a virtual X server and clipboard manager to exercise the clipboard feature
# (the manager persists the selection after srcml exits so --to-clipboard output
# can be read back)
RUN apt-get update \
    && apt-get install --no-install-recommends -y xvfb parcellite \
    && rm -rf /var/lib/apt/lists/*

# clipboard round-trip through srcml itself: --to-clipboard writes srcML, then
# --from-clipboard reads it back (autodetected as srcML) and the recovered
# source must match the original (needs an X server and a clipboard manager)
RUN Xvfb :99 -screen 0 1024x768x24 >/dev/null 2>&1 & \
    sleep 2; export DISPLAY=:99; parcellite >/dev/null 2>&1 & \
    sleep 2; \
    srcml --text='int n;' -l C++ --to-clipboard; \
    sleep 1; \
    out="$(srcml --from-clipboard)"; \
    { echo '=== clipboard round-trip (--to-clipboard then --from-clipboard) ==='; printf '%s\n' "$out"; } >> "$INSTALLCHECK_LOG"; \
    [ "$out" = 'int n;' ] \
    || { printf 'clipboard round-trip failed: %s\n' "$out"; exit 1; }

# print the captured outputs
RUN cat "$INSTALLCHECK_LOG"
