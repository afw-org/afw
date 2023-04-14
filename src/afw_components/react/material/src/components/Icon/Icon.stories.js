// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Icon} from "./Icon";

export default {
    title: "material / content / Icon",
    component: Icon,
    argTypes: {
        iconName: {
            name: "iconName",
            type: {
                name: "string",
                required: false,
            },
            description: "The name of the icon to render.",
        },
    }
};


const Template = (args) => <Icon {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Icon.args = {
    iconName: "string",
};
