// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Tree} from "./Tree";

export default {
    title: "material / structured / Tree",
    component: Tree,
    argTypes: {
        children: {
            name: "children",
            type: {
                name: "array",
                required: true,
            },
            description: "The children of this Tree.",
        },
        dense: {
            name: "dense",
            type: {
                name: "boolean",
                required: false,
            },
            description: "The tree nodes should be condensed in space.",
        },
        expandedKeys: {
            name: "expandedKeys",
            type: {
                name: "array",
                required: false,
            },
            description: "The keys of nodes in this tree that are expanded.",
        },
        selectedKeys: {
            name: "selectedKeys",
            type: {
                name: "array",
                required: false,
            },
            description: "The keys of nodes in this tree that are selected.",
        },
    }
};


const Template = (args) => <Tree {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Tree.args = {
    children: [{"icon": "string", "key": "string", "label": "string"}],
    dense: true,
    expandedKeys: [ "abc", "xyz", "some string" ],
    selectedKeys: [ "abc", "xyz", "some string" ],
};
