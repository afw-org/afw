// See the 'COPYING' file in the project root for licensing information.
import { useRef, useState, forwardRef } from "react";

import Box from "@mui/material/Box";
import Divider from "@mui/material/Divider";
import Icon from "@mui/material/Icon";
import Paper from "@mui/material/Paper";
import Popper from "@mui/material/Popper";
import ListItemText from "@mui/material/ListItemText";
import ListItemIcon from "@mui/material/ListItemIcon";
import MuiMenuList from "@mui/material/MenuList";
import MuiMenuItem from "@mui/material/MenuItem";
import Typography from "@mui/material/Typography";
import ClickAwayListener from "@mui/material/ClickAwayListener";

import ArrowDropDown from "@mui/icons-material/ArrowDropDown";
import Check from "@mui/icons-material/Check";
import {useTheme} from "@mui/material/styles";

const MenuHeader = ({ label }) => {
  
    return (
        <Typography 
            color="secondary" 
            variant="body2"
            sx={{ lineHeight: 2, marginLeft: 1 }}
        >
            {label}
        </Typography>
    );
};

const MenuItem = (props) => {

    const { 
        canCheck, 
        checked, 
        color, 
        disabled, 
        label, 
        description, 
        icon, 
        onClick 
    } = props;

    return (
        <MuiMenuItem 
            disabled={disabled} 
            dense
            onClick={event => {
                if (onClick)
                    onClick(event, props);
            }}
        >
            <Box>
                <Box sx={{ display: "flex" }}>
                    {
                        (canCheck && checked) && (          
                            <ListItemIcon>
                                <Check />
                            </ListItemIcon>
                        )
                    }
                    {
                        icon && (
                            <ListItemIcon sx={{ marginRight: 1 }}>
                                <Icon fontSize="small" color={color}>{ icon }</Icon>
                            </ListItemIcon>
                        )
                    }   
                    <ListItemText inset={canCheck && !checked}>{label}</ListItemText>             
                </Box>
                {
                    description && (
                        <Box sx={{ marginLeft: canCheck ? 4.5 : undefined, maxWidth: "1200px", whiteSpace: "pre-wrap" }}>
                            <Typography variant="caption" color="textSecondary">{ description }</Typography>
                        </Box>
                    )
                }
            </Box>
        </MuiMenuItem>
    );
};

const MenuItems = ({ items }) => {
    return items.map((item) => {
        if (item.type === "divider") 
            return <Divider key={item.key} />;
        else if (item.type === "header")
            return <MenuHeader {...item} />;
        else if (item.subMenu) 
            // eslint-disable-next-line @typescript-eslint/no-use-before-define
            return <SubMenu {...item} />;
        else 
            return <MenuItem {...item} />;
    });
};

const MenuList = forwardRef((props, ref) => {
    const theme = useTheme();

    const { open, placement, items, autoFocus, anchorEl, anchorPosition } = props;

    return (
        <Popper        
            open={open}    
            anchorEl={anchorEl || ({
                getBoundingClientRect() {
                    return {
                        top: anchorPosition.top,
                        left: anchorPosition.left,
                        bottom: anchorPosition.bottom,
                        right: anchorPosition.right,
                        width: 0,
                        height: 0,
                    };
                },
            })}   
            style={{ zIndex: 1500 }}       
            ref={ref}
            placement={placement || (theme?.direction === "rtl" ? "left" : "right-start")}
            modifiers={[
                {
                    name: "flip",
                    enabled: true
                },
                {
                    name: "preventOverflow",
                    enabled: true,
                    options: {
                        boundariesElement: "viewport"
                    }
                },
            ]}
        >
            <Paper elevation={4} style={{ maxHeight: "calc(100% - 32px)", overflowY: "auto" }}>
                <MuiMenuList 
                    dense
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={autoFocus}
                    autoFocusItem={autoFocus} 
                >
                    <MenuItems 
                        items={items}                         
                    />
                </MuiMenuList>
            </Paper>
        </Popper>
    );
});

const SubMenu = (props) => {
    const [open, setOpen] = useState(false);
    const menuRef = useRef(null);
    const theme = useTheme();

    const { label, disabled, subMenu, onClick } = props;

    return (
        <MuiMenuItem
            ref={menuRef}
            disabled={disabled}
            onMouseEnter={() => setOpen(true)}
            onMouseLeave={() => setOpen(false)}
            onFocus={() => setOpen(true)}
            onBlur={() => setOpen(false)}
            onClick={event => {
                setOpen(true);
                if (onClick)
                    onClick(event, props);
            }}
            dense            
        > 
            <Box sx={{ width: "100%", display: "flex" }}>
                <Box sx={{ flex: 1 }}>
                    <ListItemText>{label}</ListItemText>                  
                </Box>
                <ArrowDropDown 
                    sx={{ 
                        transform: "rotate(-90deg)", 
                        marginLeft: "auto" 
                    }} 
                />
            </Box>
            <MenuList
                open={open}
                anchorEl={menuRef.current}
                placement={theme?.direction === "rtl" ? "left" : "right-start"}
                items={subMenu.items}
            />      
        </MuiMenuItem>
    );
};

export const Menu = forwardRef((props, ref) => {
    const { 
        open, 
        items = [], 
        target, 
        onClose,
        id,
        anchorPosition,
        placement,
        "data-testid": dataTestId,
        "data-component-type": dataComponentType,
    } = props;

    if (!open) return null;

    const onClickAway = () => undefined;
    
    return (
        <ClickAwayListener onClickAway={onClose || onClickAway}>
            <MenuList
                ref={ref}
                id={id}
                data-testid={dataTestId}
                data-component-type={dataComponentType}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus
                open={open}
                anchorPosition={anchorPosition}
                anchorEl={target}
                placement={placement || "bottom-start"}
                items={items}
            />
        </ClickAwayListener>
    );
});

export default Menu;
