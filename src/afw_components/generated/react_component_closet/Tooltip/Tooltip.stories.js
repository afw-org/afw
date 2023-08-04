import {Tooltip} from "./Tooltip";

export default {
    title: "core / utilities / Tooltip",
    component: Tooltip,
    argTypes: {
        content: {
            name: "content",
            type: {
                name: "string",
                required: false,
            },
            description: "The content to display in the Tooltip.",
        },
        delay: {
            name: "delay",
            type: {
                name: "number",
                required: false,
            },
            description: "The time delay, in milliseconds, before displaying tooltip.",
        },
    }
};


const Template = (args) => <Tooltip {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Tooltip.args = {
    content: "string",
    delay: 1,
};
