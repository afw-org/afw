// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IStepperProps {
    /**
     * activeStep
     * Data Type: (integer)
     * 
     * The step that is actively displayed.
     */
    activeStep?:                        number;
    /**
     * steps
     * Data Type: (list, string)
     * 
     * The list of steps to be displayed for a the Stepper component.
     */
    steps?:                             string[];
}

/**
 *
 * A component that displays a Stepper indicating a list of steps and the
 * current one you are on.
 * 
 * This component is useful for creating a step-by-step wizard, with each
 * step providing a title, its contents, and a Next button to get to the next
 * step.
 * 
 */
export default function Stepper(props: IStepperProps): JSX.Element;
