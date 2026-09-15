// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for CodeEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./CodeEditor.propTypes";
import fallback from "./CodeEditor.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


/**
 * Implementation Id : CodeEditor
 * Category          : editors
 * 
 * A component that renders a rich text editor.
 * 
 * This component displays a rich text editor that allows the user to
 * manually edit the source code for a particular piece of data.
 * 
 */
export const CodeEditor = (props) => {

    props = {...defaultProps, ...props};

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "CodeEditor")}
            fallback={fallback}
            layoutComponent={{
                componentType: "CodeEditor",
                parameters: props
            }}
        />
    );
};


CodeEditor.propTypes = propTypes;
CodeEditor.displayName = "CodeEditor";

export default memo(CodeEditor);
