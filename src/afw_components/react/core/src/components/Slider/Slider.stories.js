// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Slider} from "./Slider";

export default {
    title: "core / inputs / Slider",
    component: Slider,
    argTypes: {
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Slider component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Slider component.",
        },
        max: {
            name: "max",
            type: {
                name: "number",
                required: false,
            },
            description: "The maximum value for the Slider component.",
        },
        min: {
            name: "min",
            type: {
                name: "number",
                required: false,
            },
            description: "The minimum value for the Slider component.",
        },
        showNumber: {
            name: "showNumber",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Indicates whether the value of the Slider should be visible.",
        },
        step: {
            name: "step",
            type: {
                name: "number",
                required: false,
            },
            description: "The incremental step value for the Slider component.",
        },
        value: {
            name: "value",
            type: {
                name: "number",
                required: false,
            },
            description: "The value for the Slider component.",
        },
        vertical: {
            name: "vertical",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Indicates whether the Slider component is vertical oriented.",
        },
    }
};


const Template = (args) => <Slider {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Slider.args = {
    disabled: true,
    label: "string",
    max: 1,
    min: 1,
    showNumber: true,
    step: 1,
    value: 1,
    vertical: true,
};
