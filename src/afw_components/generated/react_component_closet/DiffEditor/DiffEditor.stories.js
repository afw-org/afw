import {DiffEditor} from "./DiffEditor";

export default {
    title: "core / editors / DiffEditor",
    component: DiffEditor,
    argTypes: {
        modified: {
            name: "modified",
            type: {
                name: "string",
                required: false,
            },
            description: "The modified source value for the DiffEditor component to render.",
        },
        original: {
            name: "original",
            type: {
                name: "string",
                required: false,
            },
            description: "The original source value for the DiffEditor component to render.",
        },
        showLineNumbers: {
            name: "showLineNumbers",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether the line numbers should be visible in DiffEditor component.",
        },
    }
};


const Template = (args) => <DiffEditor {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
DiffEditor.args = {
    modified: "string",
    original: "string",
    showLineNumbers: true,
};
