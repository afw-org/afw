/**
 * Typescript interface definition for propTypes
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

