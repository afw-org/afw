# Software to put back after a container rebuild

`container-custom.sh` runs every `*.sh` here in name order, after the links are
set up, and writes the output to `../install.log`. A failing entry is logged and
the rest still run.

Rules for an entry:
- Check first; install only when missing. Running it again must be cheap and safe.
- One tool per file, numbered (`10-`, `20-`, ...) for order.
- Add a row below saying what it is and why.
- Whoever installs something by hand in the container adds an entry here in the
  same step, so the next rebuild does not lose it.

Only list what the image does not provide (see `.devcontainer/Dockerfile`).

Example, `10-claude-code.sh`:

```bash
#!/bin/bash
# Claude Code (native install). Lives in ~/.local, which a rebuild throws away.
# Its settings and memory are kept separately (.claude, .claude.json in links).
if [ -x "$HOME/.local/bin/claude" ]; then
    echo "claude: present ($("$HOME/.local/bin/claude" --version 2>/dev/null))"
    exit 0
fi
curl -fsSL https://claude.ai/install.sh | bash
```

| Entry | What | Why |
|-------|------|-----|

Not automatic, by choice: the AFW build itself. After a rebuild run
`./afwdev build --cdev` in the clone.
