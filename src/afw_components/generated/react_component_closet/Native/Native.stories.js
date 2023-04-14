import {Native} from "./Native";

export default {
    title: "core / content / Native",
    component: Native,
    argTypes: {
        tag: {
            name: "tag",
            type: {
                name: "string",
                required: false,
            },
            description: "The HTML Tag name.",
        },
    }
};


const Template = (args) => <Native {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Native.args = {
    tag: "string",
};
