#!/bin/bash
#
# Personal container setup (template). Copy it next to your afw clone, in the
# folder the Dev Container mounts at /workspaces, or run
# .devcontainer/personal/setup.sh, which does that for you. After that,
# .devcontainer/scripts/postCreateCommand.sh runs it on every container
# create, including rebuilds. Safe to run again by hand at any time.
#
# Keeps chosen parts of $HOME outside the container so a rebuild does not
# lose them. Everything lives in afw-container/ next to this script:
#
#   afw-container/links          names under $HOME to keep (one per line)
#   afw-container/home/          where the kept files and directories live
#   afw-container/bashrc         sourced from $HOME/.bashrc
#   afw-container/install/*.sh   software to put back (README there)
#
# Each name in links becomes a symlink $HOME/<name> -> home/<name>. The first
# time a name is listed, whatever $HOME has for it is copied into home/.
# Anything $HOME had in the way is moved aside to <name>.pre-link.<time>.
#
# See .devcontainer/personal/README.md in the afw repository.

set -u

here="$(cd "$(dirname "$(readlink -f "$0")")" && pwd)"
base="$here/afw-container"
store="$base/home"
now=$(date +%Y%m%d-%H%M%S)

mkdir -p "$store"

if [ -r "$base/links" ]; then
    while IFS= read -r name || [ -n "$name" ]; do
        name="${name%%#*}"
        name="$(echo "$name" | xargs)"
        [ -z "$name" ] && continue

        src="$store/$name"
        dst="$HOME/$name"

        # Already linked.
        if [ -L "$dst" ] && [ "$(readlink "$dst")" = "$src" ]; then
            continue
        fi

        # A mount point (a volume or bind mount from devcontainer.json or
        # docker-compose.yml) cannot be moved aside or replaced by a link.
        if command -v mountpoint >/dev/null && mountpoint -q "$dst"; then
            echo "container-custom: $dst is a mount point; not linked (see README, Troubleshooting)"
            continue
        fi

        # First time: keep what the container has.
        if [ ! -e "$src" ] && [ ! -L "$src" ]; then
            if [ -e "$dst" ]; then
                mkdir -p "$(dirname "$src")"
                if ! cp -a "$dst" "$src"; then
                    echo "container-custom: could not copy $dst to $src; left as is"
                    continue
                fi
                echo "container-custom: copied $dst to $src"
            else
                echo "container-custom: $name not in $store or $HOME; skipped"
                continue
            fi
        fi

        if [ -e "$dst" ] || [ -L "$dst" ]; then
            aside="$dst.pre-link.$now"
            n=1
            while [ -e "$aside" ] || [ -L "$aside" ]; do
                aside="$dst.pre-link.$now.$n"
                n=$((n + 1))
            done
            if ! mv -T "$dst" "$aside"; then
                echo "container-custom: could not move $dst aside; not linked"
                continue
            fi
            echo "container-custom: moved old $dst to $aside"
        fi
        mkdir -p "$(dirname "$dst")"
        # -T: if something recreated $dst as a directory meanwhile, fail
        # instead of putting the link inside it.
        if ! ln -sT "$src" "$dst"; then
            echo "container-custom: could not link $dst -> $src"
            continue
        fi
        echo "container-custom: linked $dst -> $src"
    done < "$base/links"
fi

# Source the personal bashrc from the image's $HOME/.bashrc.
hook="[ -r '$base/bashrc' ] && . '$base/bashrc'"
if ! grep -qxF "$hook" "$HOME/.bashrc" 2>/dev/null; then
    printf '\n# Personal settings kept outside the container (see %s)\n%s\n' \
        "$here/container-custom.sh" "$hook" >> "$HOME/.bashrc"
    echo "container-custom: added afw-container/bashrc hook to $HOME/.bashrc"
fi

# Software the image does not provide (see afw-container/install/README.md).
# Each entry checks before installing. Output goes to afw-container/install.log.
if [ -d "$base/install" ]; then
    log="$base/install.log"
    echo "=== $(date -u '+%Y-%m-%d %H:%M:%S UTC')" > "$log"
    for step in "$base"/install/*.sh; do
        [ -e "$step" ] || continue
        echo "--- $(basename "$step")" >> "$log"
        if bash "$step" >> "$log" 2>&1; then
            echo "container-custom: $(basename "$step") ok"
        else
            echo "container-custom: $(basename "$step") FAILED (see $log)"
            echo "FAILED" >> "$log"
        fi
    done
fi

echo "container-custom: the AFW build is not kept across rebuilds; run ./afwdev build --cdev"

exit 0
