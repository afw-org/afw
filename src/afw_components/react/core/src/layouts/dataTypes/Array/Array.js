// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for List
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {ArrayResponsive} from "../../objectTypes/ObjectResponsive/ArrayResponsive";


/*
 * The List is a component that wraps ListEditor to provide options
 * derived from an AfwProperty of dataType list.  It handles the modals to 
 * prompt for new values and manages the options when new list entries are added
 * and removed.
 */
export const _Array = (props) => {
    const {value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    return (
        <ArrayResponsive 
            {...props}
            {...valueMeta} 
            description={brief || description}
            value={value} 
            onChanged={onChanged} 
            onValidate={onValidate} 
        />
    );
};

_Array.displayName = "Array";

export default _Array;
