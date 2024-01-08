// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Boolean
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    Checkbox, 
    Typography, 
    useOperational
} from "@afw/react";

/**
 * Handles dataType=boolean
 */
export const _Boolean = (props) => {
    const {editable} = useOperational(props);
    const {id, "aria-label": ariaLabel, value, defaultValue, valueMeta = {}, onChanged} = props;
    const {brief, description} = valueMeta;

    if (editable)
        return (
            <Checkbox 
                id={id} 
                {...valueMeta} 
                description={brief || description}
                value={(value !== undefined) ? value : defaultValue} 
                onChanged={onChanged} 
            />
        );

    else
        return <Typography aria-label={ariaLabel} text={(value !== undefined) ? String(value) : ""} />;    
};

_Boolean.displayName = "Boolean";

export default _Boolean;
