// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Toolbar} from "./Toolbar";

export default {
    title: "material / containers / Toolbar",
    component: Toolbar,
    argTypes: {
        alignContent: {
            name: "alignContent",
            type: {
                name: "string",
                required: false,
            },
            description: "Defines the align-content style property.",
        },
        alignItems: {
            name: "alignItems",
            type: {
                name: "string",
                required: false,
            },
            description: "Defines the align-items style property.",
        },
        contains: {
            name: "contains",
            type: {
                name: "array",
                required: true,
            },
            description: "A list of components to be contained by this Toolbar.",
        },
        dense: {
            name: "dense",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Toolbar should be condensed as much as possible.",
        },
        justify: {
            name: "justify",
            type: {
                name: "string",
                required: false,
            },
            description: "Defines the justify-content style property.",
        },
        spacing: {
            name: "spacing",
            type: {
                name: "number",
                required: false,
            },
            description: "The spacing between items in the Toolbar.",
        },
    }
};


const Template = (args) => <Toolbar {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Toolbar.args = {
    alignContent: "stretch",
    alignItems: "flex-start",
    contains: [{}],
    dense: true,
    justify: "flex-start",
    spacing: 0,
};
