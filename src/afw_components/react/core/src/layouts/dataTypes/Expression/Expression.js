// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    TextField, 
    Typography,
    useOperational
} from "@afw/react";

/**
 * Handle dataType=expression
 */
export const Expression = (props) => {
    
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
                multiline={true}  
                value={value}
                onChanged={onChanged}
                onValidate={onValidate}
            />
        );

    else
        return <Typography text={value || ""} />;
};

Expression.displayName = "Expression";

export default Expression;
