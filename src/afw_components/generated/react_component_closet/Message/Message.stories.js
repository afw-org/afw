import {Message} from "./Message";

export default {
    title: "core / content / Message",
    component: Message,
    argTypes: {
        canDismiss: {
            name: "canDismiss",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Message can be dismissed.",
        },
        isMultiline: {
            name: "isMultiline",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Message message is multilined.",
        },
        message: {
            name: "message",
            type: {
                name: "string",
                required: false,
            },
            description: "Optional message to be displayed for the Message component.",
        },
        status: {
            name: "status",
            type: {
                name: "string",
                required: false,
            },
            description: "Status of message (error, info, warning).",
        },
    }
};


const Template = (args) => <Message {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Message.args = {
    canDismiss: true,
    isMultiline: true,
    message: "string",
    status: "error",
};
