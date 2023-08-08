import {Box} from "./Box";

export default {
    title: "core / containers / Box",
    component: Box,
    argTypes: {
        alignContent: {
            name: "alignContent",
            type: {
                name: "string",
                required: false,
            },
            description: "This aligns a flex container's lines within when there is extra space in the cross-axis, similar to how 'Justify Content' aligns individual items within the main-axis.",
        },
        alignItems: {
            name: "alignItems",
            type: {
                name: "string",
                required: false,
            },
            description: "This defines the default behavior for how flex items are laid out along the cross axis on the current line. Think of it as the 'Justify Content' version for the cross axis (perpendicular to the main-axis).",
            defaultValue: "stretch",
        },
        contains: {
            name: "contains",
            type: {
                name: "array",
                required: false,
            },
            description: "This declares a list of items to be rendered inside the Box.",
        },
        direction: {
            name: "direction",
            type: {
                name: "string",
                required: false,
            },
            description: "This declares the direction that items are oriented inside the Box.",
            defaultValue: "row",
        },
        justifyContent: {
            name: "justifyContent",
            type: {
                name: "string",
                required: false,
            },
            description: "This defines the alignment along the main axis. It helps distribute extra free space leftover when either all the flex items on a line are inflexible, or are flexible but have reached their maximum size. It also exerts some control over the alignment of items when they overflow the line.",
            defaultValue: "flex-start",
        },
        wrap: {
            name: "wrap",
            type: {
                name: "string",
                required: false,
            },
            description: "By default, flex items will all try to fit onto one line. You can change that and allow the items to wrap as needed with this property.",
            defaultValue: "nowrap",
        },
    }
};


const Template = (args) => <Box {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Box.args = {
    alignContent: "string",
    alignItems: "string",
    contains: [{"grow": 1, "order": 1, "shrink": 1}],
    direction: "string",
    justifyContent: "string",
    wrap: "string",
};
