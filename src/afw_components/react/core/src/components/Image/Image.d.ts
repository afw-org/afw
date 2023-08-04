// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IImageProps {
    /**
     * src
     * Data Type: (string)
     * 
     * The source for the image to be rendered.
     */
    src?:                               string;
}

/**
 *
 * A component that displays an image.
 * 
 * This component displays an image from a variety of sources and formats.
 * 
 */
export default function Image(props: IImageProps): JSX.Element;
