// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Date
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    DatePicker, 
    Typography,
    useOperational
} from "@afw/react";

/**
 * Handles dataType=date
 */
export const _Date = (props) => {
    
    const {editable} = useOperational(props);
    const {id, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) {
        return (
            <DatePicker 
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
        return <Typography text={value || ""} />;
    }
};

_Date.displayName = "Date";

export default _Date;
