// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Paper} from "./Paper";

export default {
    title: "material / containers / Paper",
    component: Paper,
    argTypes: {
        elevation: {
            name: "elevation",
            type: {
                name: "number",
                required: false,
            },
            description: "Depth elevation appearance of the Paper surface.",
        },
        square: {
            name: "square",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Use square corners instead of rounded.",
            defaultValue: false,
        },
    }
};


const Template = (args) => <Paper {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Paper.args = {
    elevation: 1,
    square: true,
};
