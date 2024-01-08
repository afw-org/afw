// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tree
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Tree.propTypes";
import fallback from "./Tree.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Tree
 * Category          : structured
 * 
 * A component that renders data in a Tree structure.
 * 
 * This component helps to organize content into a Tree structure, where
 * nodes can be selected, expanded, or collapsed to allow the user to
 * navigate more complex nested data or component structures.
 * 
 */
export const Tree = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Tree")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Tree",
                parameters: props
            }}
        />
    );
};


Tree.propTypes = propTypes;
Tree.defaultProps = defaultProps;
Tree.displayName = "Tree";

export default memo(Tree);
