FROM ghcr.io/afw-org/afw-dev-base:latest

# Allow bash history to persist between container rebuilds by sending it to a directory that can reside on a persisted volume
# Also, alias ls to show colors by default
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
    && echo $SNIPPET >> "/root/.bashrc" \
    && echo "alias ls='ls --color=auto'" >> "/root/.bashrc"
