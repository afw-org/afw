import {Link} from "./Link";

export default {
    title: "core / navigation / Link",
    component: Link,
    argTypes: {
        disabled: {
            name: "disabled",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the the Link is disabled.",
        },
        text: {
            name: "text",
            type: {
                name: "string",
                required: false,
            },
            description: "The text value displayed for the Link component.",
        },
        url: {
            name: "url",
            type: {
                name: "string",
                required: false,
            },
            description: "The URL for the link.",
        },
    }
};


const Template = (args) => <Link {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Link.args = {
    disabled: true,
    text: "string",
    url: "string",
};
