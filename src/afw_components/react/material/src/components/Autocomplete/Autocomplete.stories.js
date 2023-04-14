// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Autocomplete} from "./Autocomplete";

export default {
    title: "material / inputs / Autocomplete",
    component: Autocomplete,
    argTypes: {
        allowWrite: {
            name: "allowWrite",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Autocomplete component can be changed.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Autocomplete component is disabled.",
            defaultValue: false,
        },
        isLoading: {
            name: "isLoading",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Autocomplete component is currently loading.",
            defaultValue: false,
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Autocomplete component.",
        },
        options: {
            name: "options",
            type: {
                name: "array",
                required: false,
            },
            description: "The available options for the Autocomplete component.",
        },
        placeholder: {
            name: "placeholder",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies the placeholder text displayed when no value is present.",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this Autocomplete component is required.",
        },
        value: {
            name: "value",
            type: {
                name: "object",
                required: false,
            },
            description: "The option value for the Autocomplete component.",
        },
    }
};


const Template = (args) => <Autocomplete {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Autocomplete.args = {
    allowWrite: true,
    disabled: true,
    isLoading: true,
    label: "string",
    options: [{"key": "string", "text": "string"}],
    placeholder: "string",
    required: true,
    value: {
        key: "string",
        text: "string",
    },
};
