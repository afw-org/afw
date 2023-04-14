// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for YearMonthDuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    Typography, 
    TextField,
    useOperational,
} from "@afw/react";

/**
 * Handles dataType=yearMonthDuration
 */
export const YearMonthDuration = (props) => {
    
    const {editable} = useOperational(props);
    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
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
        return <Typography text={value || ""} />;

};

YearMonthDuration.displayName = "YearMonthDuration";

export default YearMonthDuration;
