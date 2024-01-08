// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for EditableCallout
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./EditableCallout.propTypes";
import fallback from "./EditableCallout.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : EditableCallout
 * Category          : editors
 * 
 * A component that calls out with an editable multiline text area.
 * 
 * This component wraps an editable text field inside a Callout component.
 * This is useful when a label needs to be quickly edited by popping up an
 * editor, rather than navigating them to a new page.  The EditableLabel
 * component that it displays may be a multiline text field.
 * 
 */
export const EditableCallout = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "EditableCallout")}
            fallback={fallback}
            layoutComponent={{
                componentType: "EditableCallout",
                parameters: props
            }}
        />
    );
};


EditableCallout.propTypes = propTypes;
EditableCallout.defaultProps = defaultProps;
EditableCallout.displayName = "EditableCallout";

export default memo(EditableCallout);
