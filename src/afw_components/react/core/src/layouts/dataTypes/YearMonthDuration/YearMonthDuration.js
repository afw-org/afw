// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for YearMonthDuration
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {Typography} from "../../../components/Typography/Typography";
import {TextField} from "../../../components/TextField/TextField";
import {useOperational} from "../../../hooks";

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
