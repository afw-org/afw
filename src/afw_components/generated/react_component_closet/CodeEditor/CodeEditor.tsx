// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for CodeEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./CodeEditor.propTypes";
import {ICodeEditorProps} from "./CodeEditor.types";
import fallback from "./CodeEditor.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : CodeEditor
 * Category          : editors
 * 
 * A component that renders a rich text editor.
 * 
 * This component displays a rich text editor that allows the user to manually
 * edit the source code for a particular piece of data.
 * 
 */
export const CodeEditor : React.FunctionComponent<ICodeEditorProps> = (props) => {

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
CodeEditor.defaultProps = defaultProps;
CodeEditor.displayName = "CodeEditor";

export default React.memo(CodeEditor);
