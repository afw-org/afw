// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for List
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {ListResponsive} from "../../objectTypes/ObjectResponsive/ListResponsive";


/*
 * The List is a component that wraps ListEditor to provide options
 * derived from an AfwProperty of dataType list.  It handles the modals to 
 * prompt for new values and manages the options when new list entries are added
 * and removed.
 */
export const List = (props) => {
    const {value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    return (
        <ListResponsive 
            {...props}
            {...valueMeta} 
            description={brief || description}
            value={value} 
            onChanged={onChanged} 
            onValidate={onValidate} 
        />
    );
};

List.displayName = "List";

export default List;
