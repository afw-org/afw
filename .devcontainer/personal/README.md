# Keeping personal settings across container rebuilds

A Dev Container rebuild starts from a fresh image. Everything under `$HOME` in
the container (`/root`) is lost: shell settings, CLI logins (`gh`), tool
installs, AI assistant settings and memory, `afw` history. The folder that
holds your clone is mounted at `/workspaces`, so it survives a rebuild.

`.devcontainer/scripts/postCreateCommand.sh` already runs
`/workspaces/container-custom.sh` after every container create, if that file
exists and is executable. The files here are a ready-made version of that
script, plus a place to keep what it restores. Nothing here runs unless you
set it up.

## Setup

In the container's terminal, from the clone:

```bash
.devcontainer/personal/setup.sh
```

It copies the templates next to the clone (never overwriting) and runs the
script once. You end up with:

| Path in the container | What |
|-----------------------|------|
| `/workspaces/container-custom.sh` | The script the hook runs. Yours to change. |
| `/workspaces/afw-container/links` | Names under `$HOME` to keep, one per line. Starts all commented out. |
| `/workspaces/afw-container/home/` | Where the kept files and directories live. |
| `/workspaces/afw-container/bashrc` | Your shell settings, sourced from `~/.bashrc`. |
| `/workspaces/afw-container/install/` | `*.sh` entries that put back software the image lacks. |

On the host these are next to your clone, for example `~/afw-container` when
the clone is `~/afw`.

## Keeping something new

1. Add its name (relative to `$HOME`) to `afw-container/links`, for example
   `.config/gh`.
2. Run `/workspaces/container-custom.sh`. The first time a name is listed, the
   container's copy moves into `afw-container/home/`, and `$HOME/<name>`
   becomes a symlink to it.

Prefer directories over single files. For software installed outside the
listed names (for example under `~/.local`), add an entry to
`afw-container/install/` instead; see the README there.

The AFW build is not kept. After a rebuild run `./afwdev build --cdev`.

## Troubleshooting

A shell that starts with
`afw-container: /root/<name> is not linked; run /workspaces/container-custom.sh`
means `$HOME/<name>` is no longer the symlink. Changes made there since then
will be lost at the next rebuild. Run the script; if the message stays, check:

- **Something is mounted there.** `findmnt $HOME/<name>` prints a line when a
  volume or bind mount sits on that path. A mount point cannot be moved aside
  or replaced by a symlink; the script says so and skips it. Look for the
  mount in `.devcontainer/devcontainer.json` (`mounts`) and
  `docker-compose.yml` (`volumes`).
- **A local edit git is hiding.** `git ls-files -v .devcontainer` prints `S`
  for a file marked skip-worktree. Such a file can differ from what is
  committed while `git status` and `git diff` show nothing. Compare with
  `git show HEAD:<path> | diff - <path>`, then clear the mark with
  `git update-index --no-skip-worktree <path>`.
- **A tool replaced the link.** Some programs save a file by writing a
  temporary file and renaming it over the original, which turns the symlink
  into a plain file. The script moves that file aside to
  `<name>.pre-link.<time>` and links again; copy over anything newer from it.
  Keeping the directory around the file instead avoids this.
- **The hook did not run.** The script must be executable
  (`chmod +x /workspaces/container-custom.sh`). Its output is in the Dev
  Containers log of the rebuild.

## Other setups

- **Clone into a container volume:** `/workspaces` is that volume, so
  `afw-container/` survives rebuilds as long as the volume is kept.
- **GitHub Codespaces:** not tried. Check whether `/workspaces` outside the
  clone is kept across a rebuild before relying on it.
- **Non-root user:** the script uses `$HOME`, so it should work, but it has
  only been used as root.
