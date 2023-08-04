// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Nav} from "./Nav";

export default {
    title: "material / navigation / Nav",
    component: Nav,
    argTypes: {
        items: {
            name: "items",
            type: {
                name: "array",
                required: false,
            },
            description: "Item to be displayed by this Nav layout.",
        },
    }
};


const Template = (args) => <Nav {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Nav.args = {
    items: [{"icon": "string", "isExpanded": true, "label": "string", "selected": true, "url": "string"}],
};
