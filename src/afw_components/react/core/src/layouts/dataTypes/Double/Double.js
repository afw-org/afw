// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Double
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    TextField, 
    Typography,
    useOperational,
} from "@afw/react";

/**
 * Handles dataType=double
 */
export const Double = (props) => {
    
    const {editable} = useOperational(props);
    const {id, ariaLabel, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    /* when TextField is changed, parse it back to numeric value */
    const _onChanged = (newValue) => {
        if (onChanged) {
            const val = parseInt(newValue, 10);
            if (isNaN(val))
                onChanged();
            else
                onChanged(val);
        }
    };

    const strValue = (value === undefined) ? "" : String(value);

    if (editable)
        return (
            <TextField 
                id={id}
                aria-label={ariaLabel}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta} 
                description={brief ? brief : description}
                type="number" 
                value={strValue} 
                onChanged={_onChanged} 
                onValidate={onValidate} 
            />
        );
    else
        return <Typography aria-label={ariaLabel} text={strValue} />;    
};

Double.displayName = "Double";

export default Double;
