// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for String
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {TextField} from "../../../components/TextField/TextField";
import {Typography} from "../../../components/Typography/Typography";
import {useOperational} from "../../../hooks";

/**
 * Handles dataType=null
 */
export const Null = (props) => {

    const {editable} = useOperational(props);
    const {id, valueMeta = {}} = props;
    const {brief, label, description} = valueMeta;
    
    if (!editable) {
        return <Typography text="null" />;
    } else {
        return (
            <TextField 
                id={id}
                label={label}
                description={brief ? brief : description}                
                type="text"
                value="null"
                disabled
            />
        );
    }
};

Null.displayName = "Null";

export default Null;
