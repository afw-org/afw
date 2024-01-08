// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Image
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Image.propTypes";
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
export const Image = (props) => {

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

export default memo(Image);
