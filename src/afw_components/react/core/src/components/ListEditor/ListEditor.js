// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ListEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./ListEditor.propTypes";
import fallback from "./ListEditor.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : ListEditor
 * Category          : editors
 * 
 * A component that allows the user to manage a list of text items.
 * 
 * This component renders a list of text and optional secondary text, items,
 * which allows the user to add, edit or delete items from the list.
 * 
 */
export const ListEditor = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "ListEditor")}
            fallback={fallback}
            layoutComponent={{
                componentType: "ListEditor",
                parameters: props
            }}
        />
    );
};


ListEditor.propTypes = propTypes;
ListEditor.defaultProps = defaultProps;
ListEditor.displayName = "ListEditor";

export default memo(ListEditor);
