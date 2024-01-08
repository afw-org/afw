// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Spinner
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Spinner.propTypes";
import {ISpinnerProps} from "./Spinner.types";
import fallback from "./Spinner.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Spinner
 * Category          : utilities
 * 
 * A component that displays a spinner progress indicator.
 * 
 * This utility component displays a spinning circle to indicate to the user
 * to wait for processing to complete.
 * 
 */
export const Spinner : React.FunctionComponent<ISpinnerProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Spinner")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Spinner",
                parameters: props
            }}
        />
    );
};


Spinner.propTypes = propTypes;
Spinner.defaultProps = defaultProps;
Spinner.displayName = "Spinner";

export default React.memo(Spinner);
