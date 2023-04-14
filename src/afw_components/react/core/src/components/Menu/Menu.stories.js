// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Menu} from "./Menu";

export default {
    title: "core / navigation / Menu",
    component: Menu,
    argTypes: {
        items: {
            name: "items",
            type: {
                name: "array",
                required: false,
            },
            description: "Item to display under this Menu.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether this menu is open.",
        },
    }
};


const Template = (args) => <Menu {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Menu.args = {
    items: [{"color": "string", "disabled": true, "icon": "string", "label": "string", "type": "string"}],
    open: true,
};
