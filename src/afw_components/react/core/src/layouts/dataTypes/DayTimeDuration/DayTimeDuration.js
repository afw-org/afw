// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DayTimeDuration
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    TextField, 
    Typography,
    useOperational
} from "@afw/react";

/**
 * Handles dataType=dayTimeDuration
 */
export const DayTimeDuration = (props) => {
    
    const {editable} = useOperational(props);
    const {id, autoFocus, value, "aria-label": ariaLabel, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) 
        return (
            <TextField 
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta} 
                description={brief || description}
                type="text"
                value={value} 
                onChanged={onChanged} 
                onValidate={onValidate} 
            />
        );

    else
        return (
            <Typography 
                aria-label={ariaLabel} 
                size="5"
                component="span"
                text={value || ""} 
            />
        );
};

DayTimeDuration.displayName = "DayTimeDuration";

export default DayTimeDuration;
