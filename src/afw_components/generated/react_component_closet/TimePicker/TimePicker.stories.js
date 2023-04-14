import {TimePicker} from "./TimePicker";

export default {
    title: "core / pickers / TimePicker",
    component: TimePicker,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The text description for this TimePicker component.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The text label for this TimePicker component.",
        },
    }
};


const Template = (args) => <TimePicker {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
TimePicker.args = {
    description: "string",
    label: "string",
};
