// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DateTime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    Typography, 
    DateTimePicker,
    useOperational,
} from "@afw/react";

/**
 * Handles dataType=dateTime
 */
export const DateTime = (props) => {
    
    const {editable} = useOperational(props);
    const {id, value, "aria-label": ariaLabel, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) {
        return (
            <DateTimePicker 
                id={id} 
                {...valueMeta} 
                description={brief || description}                
                value={value} 
                onChanged={onChanged} 
                onValidate={onValidate}
            />
        );
    }

    else {
        return (
            <Typography 
                aria-label={ariaLabel} 
                size="5"
                component="span"
                text={value ? value : ""} 
            />
        );
    }
};

DateTime.displayName = "DateTime";

export default DateTime;
