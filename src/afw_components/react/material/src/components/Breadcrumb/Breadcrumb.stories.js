// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Breadcrumb} from "./Breadcrumb";

export default {
    title: "material / containers / Breadcrumb",
    component: Breadcrumb,
    argTypes: {
    }
};


const Template = (args) => <Breadcrumb {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Breadcrumb.args = {
    items: [
        { text: "Link 1" },
        { text: "Link 2" },
        { text: "Link 3" },
    ]
};
