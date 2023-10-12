# Build stage for Javascript compiles (only build on native platform for JS)
FROM --platform=$BUILDPLATFORM ghcr.io/afw-org/afw-dev-base:alpine AS build-stage

COPY ./ /src
COPY ./docker/images/builder/builder-alpine.sh /builder.sh

RUN chmod +x /builder.sh && BUILD_TARGET=js /builder.sh


FROM nginx:latest

LABEL org.opencontainers.image.source https://github.com/afw-org/afw

# builder images must already be created to grab artifacts
COPY --from=build-stage /afw-apps-*.tar /usr/share/nginx/html
COPY --from=build-stage /afw-docs-*.tar /usr/share/nginx/html

RUN cd /usr/share/nginx/html && tar xvf afw-apps-*.tar && rm -f afw-apps-*.tar && \
    cd /usr/share/nginx/html && tar xvf afw-docs-*.tar && rm -f afw-docs-*.tar

# copy nginx.conf configuration
COPY docker/images/afw-admin/nginx.conf /etc/nginx/