// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {TimezonePicker} from "./TimezonePicker";

export default {
    title: "core / pickers / TimezonePicker",
    component: TimezonePicker,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The text description for this TimezonePicker component.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The text label for this TimezonePicker component.",
        },
    }
};


const Template = (args) => <TimezonePicker {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
TimezonePicker.args = {
    description: "string",
    label: "string",
};
