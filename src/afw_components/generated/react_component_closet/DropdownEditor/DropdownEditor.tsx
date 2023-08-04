// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DropdownEditor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./DropdownEditor.propTypes";
import {IDropdownEditorProps} from "./DropdownEditor.types";
import fallback from "./DropdownEditor.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : DropdownEditor
 * Category          : editors
 * 
 * A component that manages a list of options in a Dropdown picker.
 * 
 * This component, given a set of options, provides the user with the ability
 * to select, edit, add or delete one of the values by clicking on the
 * dropdown icon and selecting the appropriate value and button from a list.
 * 
 */
export const DropdownEditor : React.FunctionComponent<IDropdownEditorProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "DropdownEditor")}
            fallback={fallback}
            layoutComponent={{
                componentType: "DropdownEditor",
                parameters: props
            }}
        />
    );
};


DropdownEditor.propTypes = propTypes;
DropdownEditor.defaultProps = defaultProps;
DropdownEditor.displayName = "DropdownEditor";

export default React.memo(DropdownEditor);
