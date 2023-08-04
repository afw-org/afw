// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Modal
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Modal.propTypes";
import {IModalProps} from "./Modal.types";
import fallback from "./Modal.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Modal
 * Category          : containers
 * 
 * A layout container that displays content inside a popup.
 * 
 * This component container is visible when a target element is clicked. It's
 * used to quickly show or grab input from the user in a popup area and can
 * be dismissed.
 * 
 */
export const Modal : React.FunctionComponent<IModalProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Modal")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Modal",
                parameters: props
            }}
        />
    );
};


Modal.propTypes = propTypes;
Modal.defaultProps = defaultProps;
Modal.displayName = "Modal";

export default React.memo(Modal);
