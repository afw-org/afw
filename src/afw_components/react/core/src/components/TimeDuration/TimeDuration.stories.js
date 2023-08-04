// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {TimeDuration} from "./TimeDuration";

export default {
    title: "core / inputs / TimeDuration",
    component: TimeDuration,
    argTypes: {
        dayTimeDuration: {
            name: "dayTimeDuration",
            type: {
                name: "string",
                required: false,
            },
            description: "The value of the TimeDuration component expressed as a dayTime.",
        },
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for the TimeDuration component that describes this input.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the the TimeDuration is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the TimeDuration component.",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the the TimeDuration is required.",
        },
        yearMonthDuration: {
            name: "yearMonthDuration",
            type: {
                name: "string",
                required: false,
            },
            description: "The value of the TimeDuration component expressed as a yearMonth.",
        },
    }
};


const Template = (args) => <TimeDuration {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
TimeDuration.args = {
    dayTimeDuration: "string",
    description: "string",
    disabled: true,
    label: "string",
    required: true,
    yearMonthDuration: "string",
};
