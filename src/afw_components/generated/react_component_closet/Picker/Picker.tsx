// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Picker
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Picker.propTypes";
import {IPickerProps} from "./Picker.types";
import fallback from "./Picker.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Picker
 * Category          : pickers
 * 
 * A component that allows one or more items to be picked from a list.
 * 
 * This component allows for zero or more items to be selected from a list.
 * When an item is selected, it appears as a tag in the selection, which may
 * also be later removed.
 * 
 */
export const Picker : React.FunctionComponent<IPickerProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Picker")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Picker",
                parameters: props
            }}
        />
    );
};


Picker.propTypes = propTypes;
Picker.defaultProps = defaultProps;
Picker.displayName = "Picker";

export default React.memo(Picker);
