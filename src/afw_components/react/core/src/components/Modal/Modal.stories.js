// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Modal} from "./Modal";

export default {
    title: "core / containers / Modal",
    component: Modal,
    argTypes: {
        isBlocking: {
            name: "isBlocking",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this Modal component is blocking or can lightly be dismissed.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: true,
            },
            description: "Specifies whether this Modal component is open.",
        },
    }
};


const Template = (args) => <Modal {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Modal.args = {
    isBlocking: true,
    open: true,
};
