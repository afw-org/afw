import {Snackbar} from "./Snackbar";

export default {
    title: "core / utilities / Snackbar",
    component: Snackbar,
    argTypes: {
        duration: {
            name: "duration",
            type: {
                name: "number",
                required: false,
            },
            description: "The amount of time, in milliseconds to display the Snackbar.",
        },
        message: {
            name: "message",
            type: {
                name: "string",
                required: false,
            },
            description: "The message to be displayed in the Snackbar component.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Snackbar is open or not.",
        },
        type: {
            name: "type",
            type: {
                name: "string",
                required: false,
            },
            description: "The type of message for the Snackbar component (error, warning, info, success).",
        },
    }
};


const Template = (args) => <Snackbar {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Snackbar.args = {
    duration: 1,
    message: "string",
    open: true,
    type: "error",
};
