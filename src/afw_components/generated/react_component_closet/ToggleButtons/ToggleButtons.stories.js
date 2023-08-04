import {ToggleButtons} from "./ToggleButtons";

export default {
    title: "core / inputs / ToggleButtons",
    component: ToggleButtons,
    argTypes: {
        exclusive: {
            name: "exclusive",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the choices are mutually exclusive.",
            defaultValue: true,
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the ToggleButtons component.",
        },
        required: {
            name: "required",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this ToggleButtons component is required.",
        },
    }
};


const Template = (args) => <ToggleButtons {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
ToggleButtons.args = {
    exclusive: true,
    label: "string",
    required: true,
};
