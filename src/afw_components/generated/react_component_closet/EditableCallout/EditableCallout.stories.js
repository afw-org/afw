import {EditableCallout} from "./EditableCallout";

export default {
    title: "core / editors / EditableCallout",
    component: EditableCallout,
    argTypes: {
        allowWrite: {
            name: "allowWrite",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the EditableCallout component is writable.",
        },
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the EditableCallout component is disabled.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: true,
            },
            description: "Specifies whether the EditableCallout component is visible.",
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
            description: "The value for the EditableCallout component.",
        },
    }
};


const Template = (args) => <EditableCallout {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
EditableCallout.args = {
    allowWrite: true,
    disabled: true,
    open: true,
    placeholder: "string",
    value: "string",
};
