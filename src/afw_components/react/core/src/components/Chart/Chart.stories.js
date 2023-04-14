// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Chart} from "./Chart";

export default {
    title: "core / structured / Chart",
    component: Chart,
    argTypes: {
        animateDuration: {
            name: "animateDuration",
            type: {
                name: "number",
                required: false,
            },
            description: "Duration in milliseconds to animate the chart data.",
        },
        data: {
            name: "data",
            type: {
                name: "array",
                required: false,
            },
            description: "The data for the Chart.",
        },
        tooltip: {
            name: "tooltip",
            type: {
                name: "boolean",
                required: false,
            },
            description: "Specifies whether or not the Chart contains tooltips.",
        },
        type: {
            name: "type",
            type: {
                name: "string",
                required: false,
            },
            description: "The type of Chart to display (bar, area, line, pie).",
        },
    }
};


const Template = (args) => <Chart {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Chart.args = {
    animateDuration: 1,
    data: [{"x": "string", "y": 1}],
    tooltip: true,
    type: "string",
};
