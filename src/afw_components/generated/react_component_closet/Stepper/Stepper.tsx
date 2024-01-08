// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Stepper
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Stepper.propTypes";
import {IStepperProps} from "./Stepper.types";
import fallback from "./Stepper.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Stepper
 * Category          : utilities
 * 
 * A component that displays a Stepper indicating a list of steps and the
 * current one you are on.
 * 
 * This component is useful for creating a step-by-step wizard, with each step
 * providing a title, its contents, and a Next button to get to the next step.
 * 
 */
export const Stepper : React.FunctionComponent<IStepperProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Stepper")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Stepper",
                parameters: props
            }}
        />
    );
};


Stepper.propTypes = propTypes;
Stepper.defaultProps = defaultProps;
Stepper.displayName = "Stepper";

export default React.memo(Stepper);
