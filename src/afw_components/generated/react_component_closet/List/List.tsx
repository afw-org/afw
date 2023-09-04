// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for List
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./List.propTypes";
import {IListProps} from "./List.types";
import fallback from "./List.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : List
 * Category          : containers
 * 
 * A layout container that renders a list of components.
 * 
 * This component is a simple container component that renders its contents in
 * a sequential, scrollable list.
 * 
 */
export const List : React.FunctionComponent<IListProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "List")}
            fallback={fallback}
            layoutComponent={{
                componentType: "List",
                parameters: props
            }}
        />
    );
};


List.propTypes = propTypes;
List.defaultProps = defaultProps;
List.displayName = "List";

export default React.memo(List);
