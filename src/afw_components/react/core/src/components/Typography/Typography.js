// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Typography
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Typography.propTypes";
import fallback from "./Typography.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Typography
 * Category          : content
 * 
 * A component that renders a text label.
 * 
 * This component is a very simple one, which renders text into a static
 * label with specified color, size or style..
 * 
 */
export const Typography = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Typography")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Typography",
                parameters: props
            }}
        />
    );
};


Typography.propTypes = propTypes;
Typography.defaultProps = defaultProps;
Typography.displayName = "Typography";

export default memo(Typography);
