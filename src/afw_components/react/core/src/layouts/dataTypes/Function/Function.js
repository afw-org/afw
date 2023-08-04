// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Function
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
 * Handle dataType=function
 */
export const _Function = (props) => {
    
    const {editable} = useOperational(props);
    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) {
        /*! \fixme return a Hybrid with restrictions */
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
    }

    else
        return <Typography text={value || ""} />;
};

Function.displayName = "Function";

export default _Function;
