// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Image
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Image.propTypes";
import {IImageProps} from "./Image.types";
import fallback from "./Image.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Image
 * Category          : content
 * 
 * A component that displays an image.
 * 
 * This component displays an image from a variety of sources and formats.
 * 
 */
export const Image : React.FunctionComponent<IImageProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Image")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Image",
                parameters: props
            }}
        />
    );
};


Image.propTypes = propTypes;
Image.defaultProps = defaultProps;
Image.displayName = "Image";

export default React.memo(Image);
