// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {List} from "./List";

export default {
    title: "core / containers / List",
    component: List,
    argTypes: {
        contains: {
            name: "contains",
            type: {
                name: "array",
                required: false,
            },
            description: "The layout components that are contained by this List component.",
        },
        selectable: {
            name: "selectable",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Can the List items be selected?",
        },
    }
};


const Template = (args) => <List {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
List.args = {
    contains: [{}],
    selectable: true,
};
