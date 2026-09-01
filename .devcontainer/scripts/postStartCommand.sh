#!/bin/bash

# We can add anything here we want to start after the container is started
# For example, we could always start nginx.

# The clangd VS Code/Cursor extension looks up the name "clangd" on PATH.
# A download under globalStorage does not count until you click "Use the
# installed version", and that clangd.path setting does not survive a
# container recreate. Put the already-downloaded binary on PATH if needed.
if ! command -v clangd >/dev/null 2>&1; then
    downloaded=$(ls -1 \
        /root/.cursor-server/data/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/*/clangd_*/bin/clangd \
        /root/.vscode-server/data/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/*/clangd_*/bin/clangd \
        2>/dev/null | sort | tail -1)
    if [ -n "$downloaded" ] && [ -x "$downloaded" ]; then
        mkdir -p /usr/local/bin
        ln -sfn "$downloaded" /usr/local/bin/clangd
        echo "Linked clangd on PATH: /usr/local/bin/clangd -> $downloaded"
    fi
fi

echo "Container started."
