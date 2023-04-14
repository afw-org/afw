import {Nav} from "./Nav";

export default {
    title: "core / navigation / Nav",
    component: Nav,
    argTypes: {
        items: {
            name: "items",
            type: {
                name: "array",
                required: false,
            },
            description: "Item to be displayed by this Nav layout.",
        },
    }
};


const Template = (args) => <Nav {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Nav.args = {
    items: [{"icon": "string", "isExpanded": true, "label": "string", "selected": true, "url": "string"}],
};
