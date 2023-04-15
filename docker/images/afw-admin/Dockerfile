# Build stage for Javascript compiles (only build on native platform for JS)
FROM --platform=$BUILDPLATFORM ghcr.io/afw-org/afw-dev-base:alpine AS build-stage

COPY ./ /src
COPY ./docker/images/builder/builder-alpine.sh /builder.sh

RUN chmod +x /builder.sh && BUILD_TARGET=js /builder.sh


FROM nginx:latest

LABEL org.opencontainers.image.source https://github.com/afw-org/afw

# builder images must already be created to grab artifacts
COPY --from=build-stage /admin-app-*.tar /var/www/html/

RUN cd /var/www/html && tar xvf admin-app-*.tar && rm -f admin-app-*.tar

# copy nginx.conf configuration
COPY docker/images/afw-admin/nginx.conf /etc/nginx/