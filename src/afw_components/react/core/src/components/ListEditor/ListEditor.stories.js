// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {ListEditor} from "./ListEditor";

export default {
    title: "core / editors / ListEditor",
    component: ListEditor,
    argTypes: {
        editable: {
            name: "editable",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this list is currently editable.",
        },
        items: {
            name: "items",
            type: {
                name: "array",
                required: false,
            },
            description: "The list of items to be managed.",
        },
    }
};


const Template = (args) => <ListEditor {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
ListEditor.args = {
    editable: true,
    items: [{"secondary": "string", "text": "string"}],
};
