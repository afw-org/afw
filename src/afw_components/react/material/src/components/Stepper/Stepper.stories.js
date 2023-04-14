// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {Stepper} from "./Stepper";

export default {
    title: "material / utilities / Stepper",
    component: Stepper,
    argTypes: {
        activeStep: {
            name: "activeStep",
            type: {
                name: "number",
                required: false,
            },
            description: "The step that is actively displayed.",
        },
        steps: {
            name: "steps",
            type: {
                name: "array",
                required: false,
            },
            description: "The list of steps to be displayed for a the Stepper component.",
        },
    }
};


const Template = (args) => <Stepper {...args} />;

export const Default = Template.bind({});

export const AllProps = Template.bind({});
Stepper.args = {
    activeStep: 1,
    steps: [ "abc", "xyz", "some string" ],
};
