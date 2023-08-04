// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {DropdownEditor} from "./DropdownEditor";

export default {
    title: "core / editors / DropdownEditor",
    component: DropdownEditor,
    argTypes: {
        description: {
            name: "description",
            type: {
                name: "string",
                required: false,
            },
            description: "The description for this dropdown editor.",
        },
        editable: {
            name: "editable",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Whether the options managed by this Dropdown Editor are editable.",
        },
        label: {
            name: "label",
            type: {
                name: "string",
                required: false,
            },
            description: "The label for the Dropdown component.",
        },
        multiselect: {
            name: "multiselect",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Whether this Dropdown allows for multiple selections.",
        },
        options: {
            name: "options",
            type: {
                name: "array",
                required: false,
            },
            description: "Specifies the available options for the dropdown.",
        },
        selectedKeys: {
            name: "selectedKeys",
            type: {
                name: "array",
                required: false,
            },
            description: "The key(s) that are selected for the Dropdown component.",
        },
    }
};


const Template = (args) => <DropdownEditor {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
DropdownEditor.args = {
    description: "string",
    editable: true,
    label: "string",
    multiselect: true,
    options: [{"key": "string", "text": "string"}],
    selectedKeys: [ "abc", "xyz", "some string" ],
};
