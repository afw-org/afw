// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for String
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
 * Handle dataType=x500Name
 */
export const X500Name = (props) => {

    const {editable} = useOperational(props);
    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    /*! \fixme maybe use regular expression here */    

    if (editable) {
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
    } else {
        return (
            <Typography 
                style={{ whiteSpace: "pre-line" }}
                size="5"
                text={value || ""}
            />
        );
    }
};

X500Name.displayName = "X500Name";

export default X500Name;
