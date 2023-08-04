// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for TextField
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./TextField.propTypes";
import {ITextFieldProps} from "./TextField.types";
import fallback from "./TextField.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : TextField
 * Category          : inputs
 * 
 * A component that renders a text field with a label and description.
 * 
 * This is a simple component for inputting text. It may display an outlined
 * or underlined field from which the user can enter text into. It may also
 * display a label and description to help the user understand its contents.
 * 
 */
export const TextField : React.FunctionComponent<ITextFieldProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "TextField")}
            fallback={fallback}
            layoutComponent={{
                componentType: "TextField",
                parameters: props
            }}
        />
    );
};


TextField.propTypes = propTypes;
TextField.defaultProps = defaultProps;
TextField.displayName = "TextField";

export default React.memo(TextField);
