// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IDiffEditorProps {
    /**
     * modified
     * Data Type: (string)
     * 
     * The modified source value for the DiffEditor component to render.
     */
    modified?:                          string;
    /**
     * original
     * Data Type: (string)
     * 
     * The original source value for the DiffEditor component to render.
     */
    original?:                          string;
    /**
     * showLineNumbers
     * Data Type: (boolean)
     * 
     * Specifies whether the line numbers should be visible in DiffEditor
     * component.
     */
    showLineNumbers?:                   boolean;
}

/**
 *
 * A component that compares two objects.
 * 
 * This component displays the differences between two objects.
 * 
 */
export default function DiffEditor(props: IDiffEditorProps): JSX.Element;
