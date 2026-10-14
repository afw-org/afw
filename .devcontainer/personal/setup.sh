#!/bin/bash
#
# One-time setup of personal container files (see README.md here). Run it in
# the Dev Container from anywhere in the afw clone:
#
#   .devcontainer/personal/setup.sh
#
# Copies the templates into the folder that holds the clone (/workspaces in
# the container): container-custom.sh, and afw-container/ with links, bashrc
# and install/README.md. Never overwrites a file that is already there. Then
# runs container-custom.sh once so the links take effect without a rebuild.

set -eu

templates="$(cd "$(dirname "$(readlink -f "$0")")" && pwd)"
clone="$(dirname "$(dirname "$templates")")"
workspace="$(dirname "$clone")"
base="$workspace/afw-container"

copy() {
    if [ -e "$2" ]; then
        echo "setup: $2 already exists; left as is"
    else
        mkdir -p "$(dirname "$2")"
        cp "$1" "$2"
        echo "setup: created $2"
    fi
}

copy "$templates/container-custom.sh" "$workspace/container-custom.sh"
chmod +x "$workspace/container-custom.sh"
copy "$templates/links.example" "$base/links"
copy "$templates/bashrc.example" "$base/bashrc"
copy "$templates/install/README.md" "$base/install/README.md"

echo "setup: edit $base/links to choose what to keep, then run $workspace/container-custom.sh"
"$workspace/container-custom.sh"
