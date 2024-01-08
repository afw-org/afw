// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Button
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Button.propTypes";
import fallback from "./Button.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Button
 * Category          : inputs
 * 
 * Component to display a Button.
 * 
 * This component encapsulates a wide range of Button features, including
 * labels, icons and colors.  The Button component may even allow for a
 * dropdown of selected actions to perform when clicked.
 * 
 */
export const Button = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Button")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Button",
                parameters: props
            }}
        />
    );
};


Button.propTypes = propTypes;
Button.defaultProps = defaultProps;
Button.displayName = "Button";

export default memo(Button);
