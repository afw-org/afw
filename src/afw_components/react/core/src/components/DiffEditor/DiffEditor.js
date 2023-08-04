// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DiffEditor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./DiffEditor.propTypes";
import fallback from "./DiffEditor.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : DiffEditor
 * Category          : editors
 * 
 * A component that compares two objects.
 * 
 * This component displays the differences between two objects.
 * 
 */
export const DiffEditor = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "DiffEditor")}
            fallback={fallback}
            layoutComponent={{
                componentType: "DiffEditor",
                parameters: props
            }}
        />
    );
};


DiffEditor.propTypes = propTypes;
DiffEditor.defaultProps = defaultProps;
DiffEditor.displayName = "DiffEditor";

export default memo(DiffEditor);
