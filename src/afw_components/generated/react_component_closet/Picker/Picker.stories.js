import {Picker} from "./Picker";

export default {
    title: "core / pickers / Picker",
    component: Picker,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The text description for this Picker component.",
        },
        items: {
            name: "items",
            type: {
                name: "array",
                required: false,
            },
            description: "Items to be displayed by this Picker layout.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The text label for this Picker component.",
        },
    }
};


const Template = (args) => <Picker {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Picker.args = {
    description: "string",
    items: [{"key": "string", "label": "string"}],
    label: "string",
};
