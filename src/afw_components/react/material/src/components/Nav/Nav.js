// See the 'COPYING' file in the project root for licensing information.
import {Fragment} from "react";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemIcon from "@mui/material/ListItemIcon";
import Icon from "@mui/material/Icon";
import Collapse from "@mui/material/Collapse";
import KeyboardArrowRight from "@mui/icons-material/KeyboardArrowRight";
import KeyboardArrowDown from "@mui/icons-material/KeyboardArrowDown";
import MuiLink from "@mui/material/Link";
import Typography from "@mui/material/Typography";

import {Link as RouterLink} from "react-router-dom";
import {useClasses} from "@afw/react";

const Styles = (theme) => ({
    ListItems: {
        width: "100%", 
        display: "flex", 
        alignItems: "center",
    },
    ListItem: {        
        paddingLeft: 16,
        paddingRight: 16,        
        "&:hover": {
            backgroundColor: theme.palette.background.paper,
        }
    },
    ListItemNested: {    
        paddingLeft: theme.spacing(4),
        paddingRight: 16,
        
        "&:hover": {
            backgroundColor: theme.palette.background.paper,
        }
    },
    ListItemNestedNested: {
        paddingLeft: theme.spacing(6),
        paddingRight: 16,
        
        "&:hover": {
            backgroundColor: theme.palette.background.paper,
        }
    },
    selected: {        
    },
    LinkText: {
        color: theme.palette.text.primary,
    },
    Link: {        
        flex: 1,
        marginTop: "4px",
        marginBottom: "4px",
    },
    InsetLink: {
        paddingLeft: "56px",
        flex: 1,
        marginTop: "4px",
        marginBottom: "4px",
    },    
});


export const Nav = (props) => {
    const classes = useClasses(Styles);    
    
    return (
        <List 
            dense
            component="nav"
            className={props.className}
            style={props.style}
            role="navigation"
        >
            <div role="menubar">
                {
                    props.items && props.items.map((item, index) => (
                        <Fragment key={index}>
                            <ListItem 
                                role="menuitem"
                                component="nav"                            
                                dense
                                aria-label={item.label}
                                disableGutters
                                selected={props.isSelected ? props.isSelected(item) : item.selected} 
                                onClick={(event) => {
                                    if (item.external)
                                        return;

                                    if (item.onGroupClick)
                                        item.onGroupClick(item);
                                    else if (props.onClick) 
                                        props.onClick(item, event);
                                    else if (item.onClick)
                                        item.onClick(event);
                                }}
                                classes={{
                                    root: (props.nested === 1) ? classes.ListItemNested : ((props.nested === 2) ? classes.ListItemNestedNested : classes.ListItem),
                                    selected: classes.selected
                                }}                                
                            >
                                <div className={classes.ListItems}>
                                    {
                                        item.icon && 
                                            <ListItemIcon>
                                                <Icon color="primary">{item.icon}</Icon>
                                            </ListItemIcon>
                                    }
                                    <div className={(item.inset === false) ? classes.Link : item.icon ? classes.Link : classes.InsetLink}>
                                        <MuiLink                                         
                                            component={item.external ? undefined : RouterLink}                                            
                                            href={item.url ? item.url : item.label}
                                            to={item.url ? item.url : item.label}
                                            underline="none"
                                            onClick={e => {           
                                                if (item.external)                                     
                                                    return;
                                                if (props.onClick)
                                                    props.onClick(item, e);
                                                else if (item.onClick)
                                                    item.onClick(e);                                                    
                                            }}
                                        >
                                            <Typography className={classes.LinkText} variant="subtitle2">{item.label}</Typography>
                                        </MuiLink>  
                                    </div>                              
                                    {
                                        item.items && (
                                            item.isExpanded ? <KeyboardArrowDown /> : <KeyboardArrowRight />
                                        )
                                    }    
                                </div>
                            </ListItem>
                            {
                                item.items && (
                                    <Collapse in={item.isExpanded} unmountOnExit>
                                        <Nav 
                                            {...props}
                                            items={item.items}
                                            nested={props.nested ? props.nested + 1 : 1}
                                        />
                                    </Collapse>
                                )
                            }
                        </Fragment>
                    ))                    
                }              
            </div>            
        </List>
    );        
};

export default Nav;
