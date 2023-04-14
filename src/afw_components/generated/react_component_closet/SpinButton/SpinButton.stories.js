import {SpinButton} from "./SpinButton";

export default {
    title: "core / inputs / SpinButton",
    component: SpinButton,
    argTypes: {
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the SpinButton component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the SpinButton component.",
        },
        max: {
            name: "max",
            type: {
                name: "number",
                required: false,
            },
            description: "The maximum value for the SpinButton component.",
        },
        min: {
            name: "min",
            type: {
                name: "number",
                required: false,
            },
            description: "The minimum value for the SpinButton component.",
        },
        step: {
            name: "step",
            type: {
                name: "number",
                required: false,
            },
            description: "The incremental step value for the SpinButton component.",
        },
        value: {
            name: "value",
            type: {
                name: "number",
                required: false,
            },
            description: "The value for the SpinButton component.",
        },
    }
};


const Template = (args) => <SpinButton {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
SpinButton.args = {
    disabled: true,
    label: "string",
    max: 1,
    min: 1,
    step: 1,
    value: 1,
};
