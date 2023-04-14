import {Drawer} from "./Drawer";

export default {
    title: "core / containers / Drawer",
    component: Drawer,
    argTypes: {
        anchor: {
            name: "anchor",
            type: {
                name: "string",
                required: false,
            },
            description: "This property describes which side of the page, the drawer should be anchored to. It may be one of 'bottom', 'top', 'left' or 'right'.",
            defaultValue: "left",
        },
        contains: {
            name: "contains",
            type: {
                name: "array",
                required: false,
            },
            description: "This declares a list of items to be rendered inside the Drawer.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: true,
            },
            description: "Specifies whether this Drawer component is open.",
        },
        variant: {
            name: "variant",
            type: {
                name: "string",
                required: false,
            },
            description: "This property describes whether the drawer should be anchored temporary, persistent or permanent. If temporary, the drawer will be dismissed when the user unblocks its focus. If persistent, the drawer will be displayed until the user manually closes it. If permanent, the drawer will always remain open.",
            defaultValue: "temporary",
        },
    }
};


const Template = (args) => <Drawer {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Drawer.args = {
    anchor: "string",
    contains: [{}],
    open: true,
    variant: "string",
};
