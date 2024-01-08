// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Integer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    TextField, 
    Typography,
    useOperational,
} from "@afw/react";

/**
 * Handles dataType=integer
 */
export const Integer = (props) => {
    
    const {editable} = useOperational(props);
    const {id, autoFocus, "aria-label": ariaLabel, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

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

    if (editable) {
        return (
            <TextField 
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta} 
                description={brief || description}
                type="number"
                value={strValue} 
                onChanged={_onChanged}
                onValidate={onValidate}
            />
        );
    }

    else {        
        return <Typography aria-label={ariaLabel} text={strValue} />;
    }
};

Integer.displayName = "Integer";

export default Integer;
