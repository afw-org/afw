# Docker Images

Some of these images are intended to be built automatically from 
workflows and be published so they are publicly available for download and use. 
However, anyone can build these images on their local workstation or Docker 
registry. 

When publishing the docker images, we use the GitHub container registry uri 
and the `afw-org` prefix:

| Folder | Image Name | Tag(s) |
|------------|--------------------------------|------|
| `afw-dev-base` | `ghcr.io/afw-org/afw-dev-base` | `latest`, `alpine`, `ubuntu`, `alpine3.16.6`, `ubuntu22.04` |
| `afw-base`     | `ghcr.io/afw-org/afw-base`     | `latest`, `alpine`, `ubuntu`, `alpine3.16.6`, `ubuntu22.04` |
| `afw-dev`      | `ghcr.io/afw-org/afw-dev`      | `latest`, `alpine3.16.6-0.9.0-0`, `ubuntu22.04-0.9.0-0`   |
| `afw-admin`    | `ghcr.io/afw-org/afw-admin`    | `latest`, `nginx1.23.1-0.9.0-0`                            |
| `afw`      | `ghcr.io/afw-org/afw`          | `latest`, `alpine3.16.6-0.9.0-0`, `ubuntu22.04-0.9.0-0`   |
| `afwfcgi`  | `ghcr.io/afw-org/afwfcgi`      | `latest`, `alpine3.16.6-0.9.0-0`, `ubuntu22.04-0.9.0-0`   |
| `builder`  | `N/A`  | `alpine3.16.6-0.9.0-0`, `alpine`, `node-0.9.0-0`, `node`  |

An example of one of the full image names, along with its description of tag parts:

```
ghcr.io/afw-org/<image-name>:<distribution><version>
```

The `distribution` refers to the Docker base OS (`alpine`, `ubuntu`, `nginx`, 
`node`) and `version` refers to the distribution version (`3.16.1`). The 
`version` may be omitted to indicate it's the "latest" tagged version for a 
particular distribution. Furthermore, the `distribution` name may also be 
omitted to indicate it's the default/preferred distribution for a particular 
image.

## Image Descriptions

Each folder inside here contains Dockerfile templates for building individual images.

* `afw-dev-base` - This image has all of the necessary base software installed 
for building Adaptive Framework on a variety of platforms. It also serves as the 
base image for the `devcontainer`, that VS Code can use for remote development. 
This would ideally only be updated when an underlying base distribution image 
changes.

* `afw-base` - This image is the base image for running Adaptive Framework.  It 
contains the bare minimum dependencies without all of the development 
requirements. This image would not be used to create instances from - instead, 
it should be used to inherit other images from, such as `afwfcgi` and `afw`.
This would ideally only be updated when an underlying base distribution image 
changes.

* `builder` - This image copies the current source into the image and does a 
full build. This image is intended to be a temporary storage image from which
artifacts can be copied out of to use for the remaining images, as well as 
for extracting and/or publishing installable packages from. It uses a multi-
stage build process to store the artifacts into a small `scratch` image.

* `afw-dev` - This image is nearly identical to `afw-dev-base`, with the addition 
of a version of `afw` installed. Where `afw-dev-base` is used to build 
Adaptive Framework core itself, `afw-dev` is used to build and develop 
extensions, layouts, etc without having to obtain or build Adaptive Framework 
core source code.

* `afw-admin` - The Administrative Web Application for Administrating an Adaptive 
Framework instance.  It's based on the `nginx` image. This image can be used to 
create container instances that only hosts the admin app. This image is useful 
if you have one or more FastCGI containers, running in orchestration, and wish 
to isolate a single admin web app to front them.

* `afwfcgi` - This image combines `base` with an installed instance of Adaptive 
Framework. It starts an instance of `afwfcgi`, the FastCGI service to handle
Adaptive Framework requests. This image can be used to create containers that 
only start the FastCGI service. In order to use this container, you will need 
to proxy it behind a HTTP service container, such as Nginx or Apache to create 
a fully functional HTTP service.

* `afw` - This image contains nginx, `afwfcgi` and a lot of configurations to 
serve up everything in one convenient image. It includes `nginx`, `afwfcgi`, 
the `admin` app and uses `supervisord` to start everything. It contains some 
built-in demo-able configurations, but can be used to stand up a production 
instance by providing your own volume/configuration.


## Ordered Steps for Building Images

**Note:** The sample command snippets that use the buildx plugin for multi-
architecture support will make use of the `--push` option. This instructs 
Docker to push the multi-arch platform images to the remote container registry
on GitHub to be stored. By default, GitHub, DockerHub, etc will store multi-
arch platform images because they have the ability to store manifest lists. 
Your local Docker Desktop currently does not, and trying to build a multi-arch
image without pushing it will only keep them around in build cache, as any 
attempt to store them with `--load` will result in the error message 

`ERROR: docker exporter does not currently support exporting manifest lists`.   

As of January, 2023, however, Docker Desktop does have an experimental 
`containerd` image storage feature to store manifests! It's not clear at the 
moment how it'll be used and when it'll be fully functional.

This is a basic walk-through for building all of the Docker images, using 
Alpine Linux as the underlying distribution.  The tag name should be based on 
the tag name of the distribution and afw version (in these examples, 
`alpine3.16.6` and `0.9.0-0`).

1. `afw-base` and `afw-dev-base`: These should be built first and in no order 
in particular.

  ```  
  docker build -f docker/images/afw-base/Dockerfile.alpine \
    -t ghcr.io/afw-org/afw-base:alpine3.16.6 \
    -t ghcr.io/afw-org/afw-base:alpine \
    docker/images/afw-base

  # Alternatively, build multi-architecture support, pushing it at 
  # build-time, which requires the Docker buildx plugin:
  docker buildx build --platform linux/amd64,linux/arm64 \
    -f docker/images/afw-base/Dockerfile.alpine \
    -t ghcr.io/afw-org/afw-base:alpine3.16.6 \
    -t ghcr.io/afw-org/afw-base:alpine \
    --push docker/images/afw-base
  
  docker build -f docker/images/afw-dev-base/Dockerfile.alpine \
    -t ghcr.io/afw-org/afw-dev-base:alpine3.16.6 \
    -t ghcr.io/afw-org/afw-dev-base:alpine \
    .

  # Alternatively, build multi-architecture support, pushing it at 
  # build-time, which requires the Docker buildx plugin:  
  docker buildx build --platform linux/amd64,linux/arm64 \
    -f docker/images/afw-dev-base/Dockerfile.alpine \  
    -t ghcr.io/afw-org/afw-dev-base:alpine3.16.6 \
    -t ghcr.io/afw-org/afw-dev-base:alpine \
    --push .
  ```

2. Next, optionally use the `ghcr.io/afw-org/afw-dev-base:alpine` image you 
created in the previous step to build Adaptive Framework into a builder image, 
if you wish to produce and distribute individual artifacts (RPM's/TAR's/etc). 
If you do not need these, then skip to the next step. 
This assumes that you are at the top-level source code. This step will actually 
*copy* all of thecurrent source code into the `afw-builder` image and run the 
full build. Make sure you do not have residual build directories already in your 
source tree (node_modules, etc.). Otherwise, the image is much larger and slower 
than it needs to be.

```
  docker build -f docker/images/builder/Dockerfile.alpine -o type=local,dest=. .

  # Alternatively, build multi-arch support, which requires the Docker buildx 
  # plugin:  
  docker buildx build --platform linux/amd64,linux/arm64 \
    -f docker/images/builder/Dockerfile.alpine -o type=local,dest=. .
```

This will produce multiple folders, `linux_amd64`, `linux_arm64`, each 
containing artifacts for that architecture. 

3. From here, you can now build the `afw` image, 
`ghcr.io/afw-org/afw:alpine3.16.6-0.9.0-0`:

```  
  docker build \    
    -t ghcr.io/afw-org/afw:alpine3.16.6-0.9.0-0 \
    -t ghcr.io/afw-org/afw:alpine \
    -t ghcr.io/afw-org/afw:latest \
    -f docker/images/afw/Dockerfile.alpine .

  # Alternatively, build multi-arch support, pushing it at build-time
  # which requires the Docker buildx plugin:
  docker buildx build --platform linux/amd64,linux/arm64 \  
    -f docker/images/afw/Dockerfile.alpine \
    -t ghcr.io/afw-org/afw:alpine3.16.6-0.9.0-0 \
    -t ghcr.io/afw-org/afw:alpine \
    -t ghcr.io/afw-org/afw:latest --push .
```

4. To build the `afw-dev` image, `ghcr.io/afw-org/afw-dev:alpine3.16.6-0.9.0-0`:

```
  docker build \    
    -t ghcr.io/afw-org/afw-dev:alpine3.16.6-0.9.0-0 \
    -t ghcr.io/afw-org/afw-dev:alpine \
    -f docker/images/afw-dev/Dockerfile.alpine .

  # Alternatively, build multi-arch support, pushing it at build-time
  # which requires the Docker buildx plugin:
  docker buildx build --platform linux/amd64,linux/arm64 \  
    -f docker/images/afw-dev/Dockerfile.alpine \
    -t ghcr.io/afw-org/afw-dev:alpine3.16.6-0.9.0-0 \
    -t ghcr.io/afw-org/afw-dev:alpine --push .
```

4. The `afwfcgi` and `afw-admin` can be build in the same way as 
`afw` in step 3.