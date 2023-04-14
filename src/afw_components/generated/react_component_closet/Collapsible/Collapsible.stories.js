import {Collapsible} from "./Collapsible";

export default {
    title: "core / containers / Collapsible",
    component: Collapsible,
    argTypes: {
        isCollapsed: {
            name: "isCollapsed",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Collapsible content is collapsed or not.",
            defaultValue: true,
        },
        showCollapse: {
            name: "showCollapse",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the Collapsible should render the collapse feature.",
            defaultValue: true,
        },
    }
};


const Template = (args) => <Collapsible {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Collapsible.args = {
    isCollapsed: true,
    showCollapse: true,
};
