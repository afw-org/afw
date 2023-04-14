// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {CodeEditor} from "./CodeEditor";

export default {
    title: "core / editors / CodeEditor",
    component: CodeEditor,
    argTypes: {
        readOnly: {
            name: "readOnly",
            type: {
                name: "boolean",
                required: false,
            },
            description: "The source value for the CodeEditor component should be read-only.",
        },
        showGutter: {
            name: "showGutter",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the gutter should be visible in CodeEditor component.",
        },
        showLineNumbers: {
            name: "showLineNumbers",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the line numbers should be visible in CodeEditor component.",
        },
        showPrintMargin: {
            name: "showPrintMargin",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the print margin should be visible in CodeEditor component.",
        },
        source: {
            name: "source",
            type: {
                name: "string",
                required: false,
            },
            description: "The source value for the CodeEditor component to render.",
        },
    }
};


const Template = (args) => <CodeEditor {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
CodeEditor.args = {
    readOnly: true,
    showGutter: true,
    showLineNumbers: true,
    showPrintMargin: true,
    source: "string",
};
