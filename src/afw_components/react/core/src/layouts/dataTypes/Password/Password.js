// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Password
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
 * Handles dataType=password
 */
export const Password = (props) => {

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
                value={value}
                type="password" 
                onChanged={onChanged} 
                onValidate={onValidate} 
            />
        );    
    else
        return <Typography text="********" />;

};

Password.displayName = "Password";

export default Password;
