# Build stage for Javascript compiles (only build on native platform for JS)
FROM --platform=$BUILDPLATFORM ghcr.io/afw-org/afw-dev-base:alpine AS build-stage

COPY ./ /src
COPY ./docker/images/builder/builder-alpine.sh /builder.sh

RUN chmod +x /builder.sh && BUILD_TARGET=js /builder.sh


FROM nginx:latest

LABEL org.opencontainers.image.source=https://github.com/afw-org/afw

# builder images must already be created to grab artifacts
COPY --from=build-stage /afw-apps-*.tar /usr/share/nginx/html
COPY --from=build-stage /afw-docs-*.tar /usr/share/nginx/html

RUN cd /usr/share/nginx/html && tar xvf afw-apps-*.tar && rm -f afw-apps-*.tar && \
    cd /usr/share/nginx/html && tar xvf afw-docs-*.tar && rm -f afw-docs-*.tar

# nginx.conf is a template the nginx image's entrypoint fills in at start
# (20-envsubst-on-templates.sh), writing /etc/nginx/nginx.conf.
COPY docker/images/afw-admin/nginx.conf.template /etc/nginx/templates/

# AFWFCGI: where afwfcgi listens, host:port or unix:/path. The default is
# a Compose service or Kubernetes Service named afwfcgi on its default
# port; Kubernetes needs the full name (afwfcgi.<namespace>.svc...:9345),
# because nginx's resolver does not use DNS search domains.
# NGINX_ENTRYPOINT_LOCAL_RESOLVERS: set NGINX_LOCAL_RESOLVERS from
# /etc/resolv.conf, for the template's resolver.
ENV AFWFCGI=afwfcgi:9345 \
    NGINX_ENTRYPOINT_LOCAL_RESOLVERS=1 \
    NGINX_ENVSUBST_OUTPUT_DIR=/etc/nginx \
    NGINX_ENVSUBST_FILTER="^(AFWFCGI|NGINX_LOCAL_RESOLVERS)$"

EXPOSE 8080