// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Toggle} from "./Toggle";

export default {
    title: "material / inputs / Toggle",
    component: Toggle,
    argTypes: {
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Toggle component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Toggle component.",
        },
        offText: {
            name: "offText",
            type: {
                name: "string",
                required: false,
            },
            description: "The text displayed when the Toggle component is off.",
        },
        onText: {
            name: "onText",
            type: {
                name: "string",
                required: false,
            },
            description: "The text displayed when the Toggle component is on.",
        },
        value: {
            name: "value",
            type: {
                name: "boolean",
                required: false,
            },
            description: "The value for the Toggle component.",
        },
    }
};


const Template = (args) => <Toggle {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Toggle.args = {
    disabled: true,
    label: "string",
    offText: "string",
    onText: "string",
    value: true,
};
