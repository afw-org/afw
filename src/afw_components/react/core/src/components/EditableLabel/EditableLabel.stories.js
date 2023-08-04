// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {EditableLabel} from "./EditableLabel";

export default {
    title: "core / editors / EditableLabel",
    component: EditableLabel,
    argTypes: {
        multiline: {
            name: "multiline",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the TextField for the EditableLabel needs to be multilined.",
        },
        placeholder: {
            name: "placeholder",
            type: {
                name: "string",
                required: false,
            },
            description: "The placeholder value for the text field when no value exists.",
        },
        value: {
            name: "value",
            type: {
                name: "string",
                required: false,
            },
            description: "The value for the EditableLabel component.",
        },
    }
};


const Template = (args) => <EditableLabel {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
EditableLabel.args = {
    multiline: true,
    placeholder: "string",
    value: "string",
};
