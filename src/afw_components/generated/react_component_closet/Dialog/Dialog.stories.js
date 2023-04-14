import {Dialog} from "./Dialog";

export default {
    title: "core / containers / Dialog",
    component: Dialog,
    argTypes: {
        fullScreen: {
            name: "fullScreen",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Should the Dialog be full screen?",
            defaultValue: false,
        },
        isBlocking: {
            name: "isBlocking",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Determines if the Dialog should block dismissing until the user clicks a button.",
        },
        maxWidth: {
            name: "maxWidth",
            type: {
                name: "string",
                required: false,
            },
            description: "The maximum width of the Dialog.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this Dialog component is open.",
        },
        showClose: {
            name: "showClose",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Should a close button be rendered, to allow the user to dismiss the Dialog.",
            defaultValue: false,
        },
        subText: {
            name: "subText",
            type: {
                name: "string",
                required: false,
            },
            description: "The content text to display inside the dialog box, directly under the Title.",
        },
        title: {
            name: "title",
            type: {
                name: "string",
                required: false,
            },
            description: "The title text to display at the top of the dialog box.",
        },
    }
};


const Template = (args) => <Dialog {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Dialog.args = {
    fullScreen: true,
    isBlocking: true,
    maxWidth: "sm",
    open: true,
    showClose: true,
    subText: "string",
    title: "string",
};
