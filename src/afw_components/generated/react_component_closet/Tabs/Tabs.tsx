// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tabs
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Tabs.propTypes";
import {ITabsProps} from "./Tabs.types";
import fallback from "./Tabs.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Tabs
 * Category          : navigation
 * 
 * A layout container that renders a series of tabs.
 * 
 * This component displays a list of components, one at a time, by containing
 * the content inside of their individual tabs. A tab has a clickable link,
 * which when clicked, will switch the content to the desired component.
 * 
 */
export const Tabs : React.FunctionComponent<ITabsProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Tabs")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Tabs",
                parameters: props
            }}
        />
    );
};


Tabs.propTypes = propTypes;
Tabs.defaultProps = defaultProps;
Tabs.displayName = "Tabs";

export default React.memo(Tabs);
