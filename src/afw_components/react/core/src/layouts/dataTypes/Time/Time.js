// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Time
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    TimePicker, 
    Typography,
    useOperational,
} from "@afw/react";

export const Time = (props) => {
    
    const {editable} = useOperational(props);
    const {id, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable)
        return (
            <TimePicker 
                id={id}
                {...valueMeta}  
                description={brief || description}
                value={value}
                onChanged={onChanged}
                onValidate={onValidate}
            />
        );

    else
        return <Typography text={value || ""} />;
};

Time.displayName = "Time";

export default Time;
