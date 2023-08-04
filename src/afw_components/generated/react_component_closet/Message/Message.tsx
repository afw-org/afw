// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Message
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Message.propTypes";
import {IMessageProps} from "./Message.types";
import fallback from "./Message.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Message
 * Category          : content
 * 
 * A component that renders a message to be displayed.
 * 
 * This component displays a text message within a surface that may alert the
 * user to additional information, errors or warnings.
 * 
 */
export const Message : React.FunctionComponent<IMessageProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Message")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Message",
                parameters: props
            }}
        />
    );
};


Message.propTypes = propTypes;
Message.defaultProps = defaultProps;
Message.displayName = "Message";

export default React.memo(Message);
