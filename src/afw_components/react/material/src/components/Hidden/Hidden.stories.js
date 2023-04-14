// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Hidden} from "./Hidden";

export default {
    title: "material / utilities / Hidden",
    component: Hidden,
    argTypes: {
        for: {
            name: "for",
            type: {
                name: "array",
                required: false,
            },
            description: "The breakpoint conditions for which the contained component will be hidden (xs, sm, md, lg, xl).",
        },
        lgDown: {
            name: "lgDown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "This content will only be displayed on large displays or smaller.",
        },
        mdDown: {
            name: "mdDown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "This content will only be displayed on medium displays or smaller.",
        },
        smDown: {
            name: "smDown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "This content will only be displayed on small displays or smaller.",
        },
        xlDown: {
            name: "xlDown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "This content will only be displayed on extra large displays or smaller.",
        },
        xsDown: {
            name: "xsDown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "This content will only be displayed on extra small displays.",
        },
    }
};


const Template = (args) => <Hidden {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Hidden.args = {
    for: [ "abc", "xyz", "some string" ],
    lgDown: true,
    mdDown: true,
    smDown: true,
    xlDown: true,
    xsDown: true,
};
