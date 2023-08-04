// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import MuiButton from "@mui/material/Button";
import ButtonBase from "@mui/material/ButtonBase";
import ButtonGroup from "@mui/material/ButtonGroup";
import Icon from "../Icon";
import MuiIconButton from "@mui/material/IconButton";
import Menu from "@mui/material/Menu";
import MenuItem from "@mui/material/MenuItem";
import Typography from "@mui/material/Typography";
import Tooltip from "@mui/material/Tooltip";

import {useClasses} from "@afw/react";

const styles = () => ({
    noTextTransform: {
        textTransform: "none"
    }   
});


const TooltipButton = ({ tooltip, children, disabled }) => {

    if (disabled || !tooltip)
        return children;

    return (
        <Tooltip title={tooltip}>
            { children }
        </Tooltip>
    );
};

export const Button = (props) => {    

    const [target, setTarget] = useState();    
    const startIcon = props.startIcon ? props.startIcon : props.icon;
    const ariaLabel = props["aria-label"] ? props["aria-label"] : props.label;

    const classes = useClasses(styles);

    const onMenuItemClick = (event, item) => {
        if (item.onClick)
            item.onClick(event);

        setTarget();
    };     

    if (props.type === "icon") {
        return (
            <>
                <TooltipButton {...props}>
                    <MuiIconButton 
                        id={props.id}
                        data-testid={props["data-testid"]}
                        data-component-type={props["data-component-type"]}
                        style={props.style}
                        className={props.className}                    
                        aria-label={ariaLabel}
                        color={props.color}
                        onClick={(event) => {
                            if (props.items)
                                setTarget(event.target);

                            if (props.onClick)
                                props.onClick(event);
                        }}
                        disabled={props.disabled}           
                        size={props.size}         
                        disableRipple={true}  
                    >
                        <Icon fontSize="inherit" iconName={props.icon} />
                    </MuiIconButton>
                </TooltipButton>
                { props.items &&
                    <Menu 
                        open={Boolean(target)}
                        style={{ padding: "0px" }}
                        anchorEl={target}
                        anchorOrigin={{
                            vertical: "bottom",
                            horizontal: "left",
                        }}
                        onClose={() => setTarget(null)}
                    >
                        {
                            props.items.map(item => 
                                <MenuItem 
                                    key={item.key}
                                    onClick={(event) => onMenuItemClick(event, item)}
                                    disabled={item.disabled}
                                >
                                    {
                                        item.icon &&
                                            <Icon 
                                                style={{ marginRight: "8px" }}
                                                fontSize={item.size ? item.size : props.size}
                                                iconName={item.icon} 
                                            />
                                    }
                                    {item.label}
                                </MenuItem>
                            )
                        }
                    </Menu> 
                }
            </>
        );
    } else if (props.type === "split") {
        return (
            <>
                <TooltipButton {...props}>
                    <ButtonGroup 
                        variant={props.variant || "outlined"}
                        color={props.color}
                        id={props.id}
                        data-testid={props["data-testid"]}
                        data-component-type={props["data-component-type"]}
                    >
                        <MuiButton                         
                            aria-label={ariaLabel}
                            variant={props.variant || "outlined"}
                            disabled={props.disabled}
                            color={props.color}
                            onClick={props.onClick}    
                            size={props.size}
                            type={props.buttonType}
                            startIcon={startIcon ?
                                <Icon
                                    style={{ marginRight: "8px" }}
                                    fontSize={props.size}
                                    iconName={startIcon}
                                /> : undefined
                                
                            }
                        >
                            {props.label}                                                  
                        </MuiButton>
                        <MuiButton                         
                            aria-label={ariaLabel + " Dropdown"}
                            color={props.color}
                            size={props.size}    
                            type={props.buttonType}
                            variant={props.variant || "outlined"}
                            disabled={props.disabled}                
                            onClick={event => setTarget(event.target)}                            
                        >
                            <Icon 
                                fontSize={props.size} 
                                variant={props.variant || "outlined"}
                                disabled={props.disabled}
                                iconName="expand_more"
                            />                    
                        </MuiButton>
                    </ButtonGroup>    
                </TooltipButton>            
                <Menu 
                    open={Boolean(target)}
                    style={{ padding: "0px" }}
                    anchorEl={target}
                    anchorOrigin={{
                        vertical: "bottom",
                        horizontal: "left",
                    }}
                    onClose={() => setTarget(null)}
                >
                    {
                        props.items.map(item => 
                            <MenuItem 
                                key={item.key}
                                onClick={(event) => onMenuItemClick(event, item)}
                                disabled={item.disabled}
                            >
                                {
                                    item.icon &&
                                        <Icon 
                                            style={{ marginRight: "8px" }}
                                            fontSize={item.size ? item.size : props.size}
                                            iconName={item.icon}
                                        />                                        
                                }
                                {item.label}
                            </MenuItem>
                        )
                    }
                </Menu>                        
            </>
        );
    } else if (props.type === "compound") {
        return (
            <TooltipButton {...props}>
                <ButtonBase 
                    id={props.id}
                    data-testid={props["data-testid"]}
                    data-component-type={props["data-component-type"]}
                    style={props.style}
                    className={props.className}
                    aria-label={ariaLabel}
                    variant={props.variant || "outlined"}
                    disabled={props.disabled}
                    color={props.color}
                    onClick={props.onClick}    
                    size={props.size}      
                    type={props.buttonType}
                >
                    <div style={{ padding: "20px" }}>
                        <div>
                            <Typography>{props.label}</Typography>
                        </div>
                        <div>
                            <Typography variant="caption">{props.secondaryText}</Typography>
                        </div>
                    </div>
                </ButtonBase>
            </TooltipButton>
        );
    } else {
        return (
            <TooltipButton {...props}>
                <MuiButton
                    id={props.id}
                    data-testid={props["data-testid"]}
                    data-component-type={props["data-component-type"]}
                    style={props.style}                
                    className={props.textTransform ? classes.noTextTransform : undefined}   
                    aria-label={ariaLabel}
                    variant={props.variant || "outlined"}
                    disabled={props.disabled}
                    color={props.color}
                    onClick={props.onClick}    
                    size={props.size}
                    type={props.buttonType}
                    startIcon={startIcon ?
                        <Icon
                            fontSize={props.size}
                            iconName={startIcon}
                        /> : undefined
                    }
                    endIcon={props.endIcon ?
                        <Icon
                            fontSize={props.size}
                            iconName={props.endIcon}
                        /> : undefined
                    }
                >
                    {props.label}
                    {
                        props.dropdown &&
                            <Icon iconName="expand_more" />
                    }
                </MuiButton>
            </TooltipButton>
        );
    }

};

export default Button;
