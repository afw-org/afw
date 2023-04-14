/**
 * Typescript interface definition for propTypes
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

