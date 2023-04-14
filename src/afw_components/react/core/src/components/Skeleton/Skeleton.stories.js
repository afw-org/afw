// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Skeleton} from "./Skeleton";

export default {
    title: "core / utilities / Skeleton",
    component: Skeleton,
    argTypes: {
        height: {
            name: "height",
            type: {
                name: "",
                required: false,
            },
            description: "The CSS height of the skeleton.",
        },
        type: {
            name: "type",
            type: {
                name: "string",
                required: false,
            },
            description: "The shape of the skeleton to display (line or circle).",
        },
        width: {
            name: "width",
            type: {
                name: "",
                required: false,
            },
            description: "The CSS width of the skeleton.",
        },
    }
};


const Template = (args) => <Skeleton {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Skeleton.args = {
    height: "unknown",
    type: "string",
    width: "unknown",
};
