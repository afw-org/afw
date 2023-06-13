// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Stepper
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import PropTypes from "prop-types";

import MuiStepper from "@mui/material/Stepper";
import MuiStep from "@mui/material/Step";
import MuiStepLabel from "@mui/material/StepLabel";

/**
 * Implementation Id : Stepper
 * Category          : utilities
 * 
 * A component that displays a Stepper indicating a list of steps and the
 * current one you are on.
 * 
 * This component is useful for creating a step-by-step wizard, with each
 * step providing a title, its contents, and a Next button to get to the next
 * step.
 * 
 */
export const Stepper = ({ className, style, activeStep, steps }) => {
    
    return (
        <MuiStepper 
            className={className}
            style={style}
            activeStep={activeStep}
        >
            {
                steps && steps.map((label, index) => 
                    <MuiStep key={index}>
                        <MuiStepLabel>{label}</MuiStepLabel>
                    </MuiStep>
                )
            }
        </MuiStepper>
    );    
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Stepper
 */
Stepper.propTypes = {
    /**
     * activeStep
     * Data Type: (integer)
     * 
     * The step that is actively displayed.
     */
    activeStep:                         PropTypes.number,
    /**
     * steps
     * Data Type: (list, string)
     * 
     * The list of steps to be displayed for a the Stepper component.
     */
    steps:                              PropTypes.arrayOf(PropTypes.string),
};

Stepper.defaultProps = {
};

export default Stepper;