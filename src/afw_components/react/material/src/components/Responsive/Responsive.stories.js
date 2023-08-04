// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Responsive} from "./Responsive";

export default {
    title: "material / containers / Responsive",
    component: Responsive,
    argTypes: {
        alignContent: {
            name: "alignContent",
            type: {
                name: "string",
                required: false,
            },
            description: "Align content.",
        },
        alignItems: {
            name: "alignItems",
            type: {
                name: "string",
                required: false,
            },
            description: "Align items.",
        },
        justify: {
            name: "justify",
            type: {
                name: "string",
                required: false,
            },
            description: "Justify content.",
        },
        rows: {
            name: "rows",
            type: {
                name: "array",
                required: true,
            },
            description: "The layout row components that are contained by this Responsive.",
        },
        spacing: {
            name: "spacing",
            type: {
                name: "number",
                required: false,
            },
            description: "Spacing between rows.",
        },
    }
};


const Template = (args) => <Responsive {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Responsive.args = {
    alignContent: "stretch",
    alignItems: "flex-start",
    justify: "flex-start",
    rows: [{}],
    spacing: 8,
};
