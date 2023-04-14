import {Image} from "./Image";

export default {
    title: "core / content / Image",
    component: Image,
    argTypes: {
        src: {
            name: "src",
            type: {
                name: "string",
                required: false,
            },
            description: "The source for the image to be rendered.",
        },
    }
};


const Template = (args) => <Image {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Image.args = {
    src: "string",
};
