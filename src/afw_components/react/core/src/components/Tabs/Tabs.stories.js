// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Tabs} from "./Tabs";

export default {
    title: "core / navigation / Tabs",
    component: Tabs,
    argTypes: {
        gapSpace: {
            name: "gapSpace",
            type: {
                name: "number",
                required: false,
            },
            description: "The amount of space between the tabs and tabbed content.",
        },
        tabs: {
            name: "tabs",
            type: {
                name: "array",
                required: false,
            },
            description: "The tabs that are defined for this Tabs component.",
        },
    }
};


const Template = (args) => <Tabs {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Tabs.args = {
    gapSpace: 1,
    tabs: [{"badge": 1, "text": "string"}],
};
