// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Button} from "./Button";

export default {
    title: "core / inputs / Button",
    component: Button,
    argTypes: {
        color: {
            name: "color",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies an optional color for this Button (primary, secondary, error).",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this Button component is disabled.",
        },
        dropdown: {
            name: "dropdown",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies an optional dropdown icon for this Button.",
        },
        icon: {
            name: "icon",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies an optional icon for this Button.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies the text on the Button.",
        },
        secondaryText: {
            name: "secondaryText",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies optional text to be added below the main text label.",
        },
        size: {
            name: "size",
            type: {
                name: "string",
                required: false,
            },
            description: "Specifies a size for the button (small)",
        },
        type: {
            name: "type",
            type: {
                name: "string",
                required: false,
            },
            description: "The type of Button (icon, split, compound, default).",
        },
    }
};


const Template = (args) => <Button {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Button.args = {
    color: "default",
    disabled: true,
    dropdown: true,
    icon: "string",
    label: "string",
    secondaryText: "string",
    size: "small",
    type: "icon",
};
