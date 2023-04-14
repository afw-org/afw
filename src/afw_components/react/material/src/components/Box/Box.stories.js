// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Box} from "./Box";

export default {
    title: "material / containers / Box",
    component: Box,
    argTypes: {
        alignItems: {
            name: "alignItems",
            type: {
                name: "string",
                required: false,
            },
            description: "This defines the default behavior for how flex items are laid out along the cross axis on the current line. Think of it as the \"Justify Content\" version for the cross axis (perpendicular to the main-axis).",
            defaultValue: "stretch",
        },
        direction: {
            name: "direction",
            type: {
                name: "string",
                required: false,
            },
            description: "This declares the direction that items are oriented inside the Box.",
            defaultValue: "row",
        },
        justifyContent: {
            name: "justifyContent",
            type: {
                name: "string",
                required: false,
            },
            description: "This defines the alignment along the main axis. It helps distribute extra free space leftover when either all the flex items on a line are inflexible, or are flexible but have reached their maximum size. It also exerts some control over the alignment of items when they overflow the line.",
            defaultValue: "flex-start",
        },
        wrap: {
            name: "wrap",
            type: {
                name: "string",
                required: false,
            },
            description: "By default, flex items will all try to fit onto one line. You can change that and allow the items to wrap as needed with this property.",
            defaultValue: "nowrap",
        },
    }
};


const Template = (args) => <Box {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Box.args = {
    alignItems: "string",
    direction: "string",
    justifyContent: "string",
    wrap: "string",
};
