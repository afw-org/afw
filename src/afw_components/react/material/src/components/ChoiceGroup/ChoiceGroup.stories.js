// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {ChoiceGroup} from "./ChoiceGroup";

export default {
    title: "material / inputs / ChoiceGroup",
    component: ChoiceGroup,
    argTypes: {
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the ChoiceGroup component.",
        },
        options: {
            name: "options",
            type: {
                name: "array",
                required: true,
            },
            description: "The available options for the ChoiceGroup component.",
        },
        orientation: {
            name: "orientation",
            type: {
                name: "string",
                required: false,
            },
            description: "The orientation of the Choice Group (horizontal or vertical).",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this ChoiceGroup component is required.",
        },
        selectedKey: {
            name: "selectedKey",
            type: {
                name: "string",
                required: false,
            },
            description: "The value of the selected key for the ChoiceGroup options.",
        },
    }
};


const Template = (args) => <ChoiceGroup {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
ChoiceGroup.args = {
    label: "string",
    options: [{"disabled": true, "key": "string", "text": "string"}],
    orientation: "horizontal",
    required: true,
    selectedKey: "string",
};
