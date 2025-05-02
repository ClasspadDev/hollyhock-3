# syntax=docker/dockerfile:1
FROM --platform=$BUILDPLATFORM ghcr.io/qbos07/cp-dockercross:v1.2.3 AS build

RUN --mount=type=bind,source=.,rw make -j && mkdir -p /sdk && cp -Pr libsdk.a include/ /sdk

FROM ghcr.io/qbos07/cp-dockercross:v1.2.3

COPY --from=build /sdk /sdk

ENV SDK_DIR=/sdk

LABEL org.opencontainers.image.source=https://github.com/ClasspadDev/hollyhock-3
LABEL org.opencontainers.image.authors="qubos@outlook.de"
LABEL org.opencontainers.image.vendor="QBos07"
