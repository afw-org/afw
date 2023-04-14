/**
 * Typescript interface definition for propTypes
 */
export interface ITypographyProps {
    /**
     * color
     * Data Type: (string)
     * 
     * The color of the text
     * 
     * The color of the text. This may be a theme definition, or an actual
     * color.
     */
    color?:                             string;
    /**
     * size
     * Data Type: (string)
     * 
     * The size of the text
     * 
     * The size of the text (1-11).
     */
    size?:                              string;
    /**
     * tag
     * Data Type: (string)
     * 
     * The html tag to produce
     * 
     * This allows you to explicitly set the underlying HTML tag in order to
     * get the semantics correct.
     */
    tag?:                               string;
    /**
     * text
     * Data Type: (string)
     * 
     * The text string to be displayed
     * 
     * The actual text content to be rendered by this component.
     */
    text:                               string;
}

