import {DateTimePicker} from "./DateTimePicker";

export default {
    title: "core / pickers / DateTimePicker",
    component: DateTimePicker,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for the DateTimePicker component.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the DateTimePicker component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the DateTimePicker component.",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Date Picker value is required.",
        },
        value: {
            name: "value",
            type: {
                name: "string",
                required: false,
            },
            description: "The value for the DateTimePicker component.",
        },
    }
};


const Template = (args) => <DateTimePicker {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
DateTimePicker.args = {
    description: "string",
    disabled: true,
    label: "string",
    required: true,
    value: "unknown",
};
