// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ListEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState} from "react";
import PropTypes from "prop-types";

import Button from "../Button";
import Typography from "../Typography";

import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemText from "@mui/material/ListItemText";
import IconButton from "@mui/material/IconButton";
import CloseIcon from "@mui/icons-material/Close";
import {useTheme} from "@mui/material/styles";

/**
 * Implementation Id : ListEditor
 * Category          : editors
 * 
 * A component that allows the user to manage a list of text items.
 * 
 * This component renders a list of text and optional secondary text, items,
 * which allows the user to add, edit or delete items from the list.
 * 
 */
export const ListEditor = (props) => {
    
    const [checked, setChecked] = useState([]);    
    const theme = useTheme();

    const handleToggle = (item, index) => {                
        const currentIndex = checked.indexOf(index);
        const newChecked = [...checked];

        /* if we are not editable, then don't change anything */
        if (props.editable === false)
            return;

        if (currentIndex === -1) {
            newChecked.push(index);
        } else {
            newChecked.splice(currentIndex, 1);
        }

        setChecked(newChecked);        
    };

    const onDeleteItems = (item) => {
        let items = [];

        if (item) 
            items = [ item ];
        else {
            checked.forEach(index => {
                items.push( props.items[index] );
            });
        }

        props.onDeleteItems(items);

        setChecked([]);
    };

    return (
        <div             
            className={props.className}
            style={props.style}
        >
            <div style={{ display: "flex", alignItems: "center" }}>                
                { props.label && (
                    (props.editable !== false) ?
                        <Typography color="textSecondary" size="2" text={props.label ? props.label : ""} />
                        :
                        <Typography color="textSecondary" size="3" text={props.label ? props.label : ""} />
                )
                }                
                <div style={{ display: "flex" }}>
                    { (props.editable !== false) &&  
                        <>
                            <Button 
                                style={{ marginLeft: "4px" }}
                                type="icon"
                                label="Add List Item"
                                icon="add"
                                color="primary"
                                onClick={props.onAddItem}
                                tooltip="Add List Item"
                            />    
                            <Button                             
                                type="icon"
                                label="Remove Selected Item"
                                icon="remove"
                                color="primary"
                                onClick={() => onDeleteItems()}
                                disabled={checked.length === 0}
                                tooltip="Remove Selected Item(s)"
                            />                        
                            <Button 
                                type="icon"    
                                label="Edit Selected Item"                            
                                icon="edit"
                                color="primary"
                                onClick={() => props.onEditItem(props.items[checked[0]])}    
                                disabled={checked.length !== 1}    
                                tooltip="Edit Selected Item"                                          
                            />                                                            
                        </>                                    
                    }  
                </div>  
            </div>
            <div style={{
                maxHeight: "20vh", overflow: "auto", 
                marginLeft: theme.spacing(2), 
                marginTop: theme.spacing(1), 
                marginBottom: theme.spacing(2)
            }}>
                <List disablePadding>
                    {
                        (props.items && props.items.length) ? props.items.map((item, index) => 
                            <ListItem 
                                dense
                                key={index} 
                                role={undefined}                                  
                                button
                                component="li"
                                onClick={() => handleToggle(item, index)} 
                                onDoubleClick={() => props.onEditItem(item)}
                                selected={checked.includes(index) ? true : false}          
                                disableGutters={true}                                      
                            >
                                <IconButton 
                                    size="small"
                                    aria-label="delete"
                                    onClick={() => onDeleteItems(item)}
                                >
                                    <CloseIcon color="error" />
                                </IconButton>
                                <ListItemText 
                                    primary={item.text} 
                                    secondary={item.secondary} 
                                />   
                            </ListItem>
                        ) :
                            <Typography color="textSecondary" text="(No Values)" />
                    }                    
                </List>  
            </div>     
            {
                (props.description && props.editable !== false) &&                
                    <Typography size="1" text={props.description ? props.description : ""} />      
            }
        </div>
    );
};

/* Generated objectType propType definition for property text */
const ListEditor_Item = {
    /**
     * Property: secondary
     * Data Type: (string)
     * 
     * The secondary text to display.
     */
    secondary:                          PropTypes.string,
    /**
     * Property: text
     * Data Type: (string)
     * 
     * The primary text to display.
     */
    text:                               PropTypes.string,
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_ListEditor
 */
ListEditor.propTypes = {
    /**
     * editable
     * Data Type: (boolean)
     * 
     * Specifies whether this list is currently editable.
     */
    editable:                           PropTypes.bool,
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_ListEditor_Item)
     * 
     * The list of items to be managed.
     */
    items:                              PropTypes.arrayOf(PropTypes.shape(ListEditor_Item)),
};

ListEditor.defaultProps = {
};

export default ListEditor;
