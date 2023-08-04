// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Checkbox} from "./Checkbox";

export default {
    title: "material / inputs / Checkbox",
    component: Checkbox,
    argTypes: {
        boxSide: {
            name: "boxSide",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies which side the checkbox is on (start or end).",
        },
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for the Checkbox component.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Checkbox component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Checkbox component.",
        },
        value: {
            name: "value",
            type: {
                name: "boolean",
                required: false,
            },
            description: "The value for the Checkbox component.",
        },
    }
};


const Template = (args) => <Checkbox {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Checkbox.args = {
    boxSide: "string",
    description: "string",
    disabled: true,
    label: "string",
    value: true,
};
