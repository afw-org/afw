// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for List
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import MuiList from "@mui/material/List";
import MuiListItem from "@mui/material/ListItem";

import {AdaptiveComponent, useClasses} from "@afw/react";

const styles = () => ({
    root: {
        paddingTop: "3px",
        paddingBottom: "3px",
    }
});

export const List = ({ 
    contains, selectable, onClick, "aria-label": ariaLabel, 
    id, "data-testid": dataTestId, "data-component-type": dataComponentType, 
    style, ...rest 
}) => {       
    const classes = useClasses(styles);

    if (!contains)
        return null;            
    
    return (
        <MuiList
            id={id}
            style={style}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            aria-label={ariaLabel}
            disablePadding={true}                    
        >
            {
                contains.map((item, index) => 
                    <MuiListItem 
                        className={classes.root}
                        key={index}
                        role={undefined}
                        button={selectable}
                        onClick={() => onClick && onClick(item, index)}      
                        disableGutters                          
                    >
                        <AdaptiveComponent {...rest} layoutComponent={item} />                        
                    </MuiListItem>
                )
            }
        </MuiList>                                    
    );
};

export default List;
