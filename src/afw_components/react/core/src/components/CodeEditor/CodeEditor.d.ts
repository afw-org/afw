// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ICodeEditorProps {
    /**
     * readOnly
     * Data Type: (boolean)
     * 
     * The source value for the CodeEditor component should be read-only.
     */
    readOnly?:                          boolean;
    /**
     * showGutter
     * Data Type: (boolean)
     * 
     * Specifies whether the gutter should be visible in CodeEditor
     * component.
     */
    showGutter?:                        boolean;
    /**
     * showLineNumbers
     * Data Type: (boolean)
     * 
     * Specifies whether the line numbers should be visible in CodeEditor
     * component.
     */
    showLineNumbers?:                   boolean;
    /**
     * showPrintMargin
     * Data Type: (boolean)
     * 
     * Specifies whether the print margin should be visible in CodeEditor
     * component.
     */
    showPrintMargin?:                   boolean;
    /**
     * source
     * Data Type: (string)
     * 
     * The source value for the CodeEditor component to render.
     */
    source?:                            string;
}

/**
 *
 * A component that renders a rich text editor.
 * 
 * This component displays a rich text editor that allows the user to
 * manually edit the source code for a particular piece of data.
 * 
 */
export default function CodeEditor(props: ICodeEditorProps): JSX.Element;
