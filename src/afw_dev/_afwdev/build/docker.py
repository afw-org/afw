#! /usr/bin/env python3

import os

from _afwdev.common import msg, package

_docker_default_platforms = [ 
    'linux/amd64', 
    'linux/arm64' 
]

_docker_afw_base_image_info = [    
    {
        'dockerFile': 'docker/images/afw-base/Dockerfile.alpine',
        'tags': [
            'latest',
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': 'docker/images/afw-base'
    }
]

_docker_afw_dev_base_image_info = [ 
    {
        'dockerFile': 'docker/images/afw-dev-base/Dockerfile.ubuntu',
        'tags': [
            'latest',
            'ubuntu'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },   
    {
        'dockerFile': 'docker/images/afw-dev-base/Dockerfile.alpine',
        'tags': [
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },    
    {
        'dockerFile': 'docker/images/afw-dev-base/Dockerfile.rockylinux',
        'tags': [
            'rockylinux'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },
    {
        'dockerFile': 'docker/images/afw-dev-base/Dockerfile.opensuse',
        'tags': [
            'opensuse'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    }
]

_docker_afw_dev_image_info = [ 
    {
        'dockerFile': 'docker/images/afw-dev/Dockerfile.ubuntu',
        'tags': [
            'latest',
            'ubuntu'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },   
    {
        'dockerFile': 'docker/images/afw-dev/Dockerfile.alpine',
        'tags': [
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },    
    {
        'dockerFile': 'docker/images/afw-dev/Dockerfile.rockylinux',
        'tags': [
            'rockylinux'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    },
    {
        'dockerFile': 'docker/images/afw-dev/Dockerfile.opensuse',
        'tags': [
            'opensuse'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    }
]

_docker_afw_image_info = [
    {
        'dockerFile': 'docker/images/afw/Dockerfile.alpine',
        'tags': [
            'latest',
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    }
]

_docker_afwfcgi_image_info = [
    {
        'dockerFile': 'docker/images/afwfcgi/Dockerfile.alpine',
        'tags': [
            'latest',
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    }
]

_docker_afw_admin_image_info = [
    {
        'dockerFile': 'docker/images/afw-admin/Dockerfile',
        'tags': [
            'latest',
            'alpine'
        ],
        'platforms': _docker_default_platforms,
        'context': '.'
    }
]

_docker_base_images = [
    [
        'ghcr.io/afw-org/afw-base',
        _docker_afw_base_image_info
    ],
    [
        'ghcr.io/afw-org/afw-dev-base',
        _docker_afw_dev_base_image_info
    ]    
]

_docker_images = [
    [
        'ghcr.io/afw-org/afw-dev',
        _docker_afw_dev_image_info
    ],
    [
        'ghcr.io/afw-org/afw',
        _docker_afw_image_info
    ],
    [
        'ghcr.io/afw-org/afwfcgi',
        _docker_afwfcgi_image_info
    ],
    [
        'ghcr.io/afw-org/afw-admin',
        _docker_afw_admin_image_info
    ]
]

# in addition to the tags specified by the info, the specific version info
# that is specified by FROM in the Dockerfile will be added to the tag list
def get_tags(options, image, image_info):

    tags = []

    os_tag = None
    for tag in image_info['tags']:        
        tags.append(image + ':' + tag)
        if not 'latest' in tag:
            os_tag = tag
    
    afw_package = package.get_afw_package(options)
    tags.append(image + ':' + os_tag + '-' + afw_package['version'])

    return tags

# This routine will build a specific docker image
def build_image(options, image, image_info):

    cmd = 'docker buildx build --file ' + image_info['dockerFile']
    for tag in get_tags(options, image, image_info):
        cmd += ' --tag ' + tag        
    cmd += ' --platform ' + ','.join(image_info['platforms'])
    cmd += ' ' + image_info['context']

    # if push option
    #   cmd += ' --push'

    msg.highlighted_info("    Running: " + cmd)
    #os.system(cmd)

# This routine will build docker images for the afw package
# It currently uses the "buildx" docker plug-in, which supports multi-
# architecture builds.  
#
# Because of this, it's required to run this on a system that has the 
# plugins installed.
def build(options):

    msg.highlighted_info("Building docker images...")

    # for now, we only know about building core afw docker images
    if not options['is_core_afw_package']:
        msg.highlighted_info("Not a core afw package. Skipping docker build.")
        return

    for image, info in _docker_images:
        msg.highlighted_info("  Building image: " + image)
        for image_info in info:            
            build_image(options, image, image_info)
