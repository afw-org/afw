// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {TextField} from "./TextField";

export default {
    title: "core / inputs / TextField",
    component: TextField,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for the TextField component that describes this input.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the the TextField is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the TextField component.",
        },
        multiline: {
            name: "multiline",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this TextField component is multilined.",
        },
        placeholder: {
            name: "placeholder",
            type: {
                name: "string",
                required: false,
            },
            description: "The placeholder text for this component.",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this TextField component is required.",
        },
        rows: {
            name: "rows",
            type: {
                name: "number",
                required: false,
            },
            description: "Specifies the number of rows to display.",
        },
        value: {
            name: "value",
            type: {
                name: "string",
                required: false,
            },
            description: "The value of the TextField component.",
        },
    }
};


const Template = (args) => <TextField {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
TextField.args = {
    description: "string",
    disabled: true,
    label: "string",
    multiline: true,
    placeholder: "string",
    required: true,
    rows: 1,
    value: "string",
};
