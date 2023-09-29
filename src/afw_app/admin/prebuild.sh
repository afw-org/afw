#!/bin/sh

# Shell script that is executed before app is built

# Copy in monaco-editor
#
# This is done, because the Monaco Editor does not ship ESM modules that 
# are compatible with the default webpack plugins that CRA provides. It 
# needs a custom webpack loader, which we cannot do without ejecting.
MONACO_EDITOR=../../../node_modules/monaco-editor
MONACO_LANGUAGES=../../../node_modules/monaco-languages
MONACO_LOADER=public/monaco-editor/min/vs/loader.js

if [ -f ${MONACO_LOADER} ]; then
    diff ${MONACO_LOADER} ${MONACO_EDITOR}/min/vs/loader.js > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        # delete previously installed copy from public/ folder
        rm -rf public/monaco-editor
    fi
fi

if [ ! -f ${MONACO_LOADER} ]; then
    echo "Copying monaco-editor to public/ directory..."

    mkdir -p public/monaco-editor/
    cp -r ../../../node_modules/monaco-editor/min public/monaco-editor/
    cp -r ../../../node_modules/monaco-editor/min-maps public/monaco-editor/
    cp -r ../../../node_modules/monaco-languages/release/min/javascript public/monaco-editor/min/vs/language/
    cp -r ../../../node_modules/monaco-languages/release/min/markdown public/monaco-editor/min/vs/language/
    cp -r ../../../node_modules/monaco-languages/release/min/perl public/monaco-editor/min/vs/language/
    cp -r ../../../node_modules/monaco-languages/release/min/python public/monaco-editor/min/vs/language/
    cp -r ../../../node_modules/monaco-languages/release/min/xml public/monaco-editor/min/vs/language/
    cp -r ../../../node_modules/monaco-languages/release/min/yaml public/monaco-editor/min/vs/language/
fi
