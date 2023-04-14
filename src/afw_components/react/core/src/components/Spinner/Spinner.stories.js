// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Spinner} from "./Spinner";

export default {
    title: "core / utilities / Spinner",
    component: Spinner,
    argTypes: {
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Spinner component.",
        },
        size: {
            name: "size",
            type: {
                name: "string",
                required: false,
            },
            description: "The size of the Spinner component.",
        },
    }
};


const Template = (args) => <Spinner {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Spinner.args = {
    label: "string",
    size: "string",
};
