import {Typography} from "./Typography";

export default {
    title: "core / content / Typography",
    component: Typography,
    argTypes: {
        color: {
            name: "color",
            type: {
                name: "string",
                required: false,
            },
            description: "The color of the text. This may be a theme definition, or an actual color.",
        },
        size: {
            name: "size",
            type: {
                name: "string",
                required: false,
            },
            description: "The size of the text (1-11).",
        },
        tag: {
            name: "tag",
            type: {
                name: "string",
                required: false,
            },
            description: "This allows you to explicitly set the underlying HTML tag in order to get the semantics correct.",
        },
        text: {
            name: "text",
            type: {
                name: "string",
                required: true,
            },
            description: "The actual text content to be rendered by this component.",
        },
    }
};


const Template = (args) => <Typography {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Typography.args = {
    color: "default",
    size: "1",
    tag: "h1",
    text: "string",
};
