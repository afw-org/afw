// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Table} from "./Table";

export default {
    title: "core / structured / Table",
    component: Table,
    argTypes: {
        columns: {
            name: "columns",
            type: {
                name: "array",
                required: true,
            },
            description: "The columns that are defined for this Table component.",
        },
        compact: {
            name: "compact",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Indicates whether this Table compact appears compact.",
        },
        isHeaderVisible: {
            name: "isHeaderVisible",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Indicates whether this Table displays column headers.",
        },
        rows: {
            name: "rows",
            type: {
                name: "array",
                required: true,
            },
            description: "The rows that are defined for this Table component.",
        },
        selectionMode: {
            name: "selectionMode",
            type: {
                name: "string",
                required: false,
            },
            description: "The selection mode for this table (none, single, multiple).",
        },
    }
};


const Template = (args) => <Table {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Table.args = {
    columns: [{"fieldName": "string", "isMultiline": true, "isResizable": true, "key": "string", "maxWidth": 1, "minWidth": 1, "name": "string"}],
    compact: true,
    isHeaderVisible: true,
    rows: [ "abc" ],
    selectionMode: "string",
};
