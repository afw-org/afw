// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ChoiceGroup
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./ChoiceGroup.propTypes";
import {IChoiceGroupProps} from "./ChoiceGroup.types";
import fallback from "./ChoiceGroup.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : ChoiceGroup
 * Category          : inputs
 * 
 * A component to display a selectable choice of options.
 * 
 * This component provides a mutually exclusive list of options, organized as
 * radio buttons, that may be selected.
 * 
 */
export const ChoiceGroup : React.FunctionComponent<IChoiceGroupProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "ChoiceGroup")}
            fallback={fallback}
            layoutComponent={{
                componentType: "ChoiceGroup",
                parameters: props
            }}
        />
    );
};


ChoiceGroup.propTypes = propTypes;
ChoiceGroup.defaultProps = defaultProps;
ChoiceGroup.displayName = "ChoiceGroup";

export default React.memo(ChoiceGroup);
