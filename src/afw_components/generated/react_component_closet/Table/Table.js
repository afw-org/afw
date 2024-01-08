// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Table
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Table.propTypes";
import fallback from "./Table.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Table
 * Category          : structured
 * 
 * A component that displays data in a table format.
 * 
 * This component arranges data into rows and columns, which can be optionally
 * sorted, selected and edited through actions.
 * 
 */
export const Table = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Table")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Table",
                parameters: props
            }}
        />
    );
};


Table.propTypes = propTypes;
Table.defaultProps = defaultProps;
Table.displayName = "Table";

export default memo(Table);
