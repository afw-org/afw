import {DatePicker} from "./DatePicker";

export default {
    title: "core / pickers / DatePicker",
    component: DatePicker,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for the DatePicker component.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the DatePicker component is disabled.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the DatePicker component.",
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
            description: "The value for the DatePicker component.",
        },
    }
};


const Template = (args) => <DatePicker {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
DatePicker.args = {
    description: "string",
    disabled: true,
    label: "string",
    required: true,
    value: "unknown",
};
