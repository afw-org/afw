// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Collapsible
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Collapsible.propTypes";
import fallback from "./Collapsible.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Collapsible
 * Category          : containers
 * 
 * A layout container that displays content that can be hidden by a
 * collapsible control.
 * 
 * This component helps organize data into a group, which can be temporarily
 * concealed or revealed by clicking on the collapse or expand icons. This may
 * be important for preserving real estate on the screen, or reducing the
 * amount of data that may be present at a given time, to avoid overloading
 * the end user with information.
 * 
 */
export const Collapsible = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Collapsible")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Collapsible",
                parameters: props
            }}
        />
    );
};


Collapsible.propTypes = propTypes;
Collapsible.defaultProps = defaultProps;
Collapsible.displayName = "Collapsible";

export default memo(Collapsible);
