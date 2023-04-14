import {Callout} from "./Callout";

export default {
    title: "core / containers / Callout",
    component: Callout,
    argTypes: {
        gapSpace: {
            name: "gapSpace",
            type: {
                name: "number",
                required: false,
            },
            description: "The gap space between target and the Callout component.",
        },
        open: {
            name: "open",
            type: {
                name: "boolean",
                required: true,
            },
            description: "Specifies whether this Callout component is open.",
        },
    }
};


const Template = (args) => <Callout {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Callout.args = {
    gapSpace: 1,
    open: true,
};
