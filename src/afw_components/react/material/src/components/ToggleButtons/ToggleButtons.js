// See the 'COPYING' file in the project root for licensing information.
//import Tooltip from "../Tooltip";
import Typography from "../Typography";

import Icon from "@mui/material/Icon";
import Tooltip from "@mui/material/Tooltip";
import ToggleButtonGroup from "@mui/material/ToggleButtonGroup";
import ToggleButton from "@mui/material/ToggleButton";

import {useClasses} from "@afw/react";

const StyledToggleButtonGroup = theme => ({
    grouped: {
        margin: theme.spacing(0.25),
        border: "none",
        padding: theme.spacing(0, 1),
        "&:not(:first-child)": {
            borderRadius: theme.shape.borderRadius,
        },
        "&:first-child": {
            borderRadius: theme.shape.borderRadius
        },
    },
});

const StyledToggleButton = () => ({    
    sizeSmall: {
        height: "36px",
        lineHeight: 0.8,
    }
});

const TooltipToggleButton = (props) => {

    const {title, ...other} = props;
    
    return (
        <Tooltip title={title}>
            <ToggleButton {...other} />
        </Tooltip>
    );
};


export const ToggleButtons = ({ onChanged, disabled, options, exclusive, value }) => {
    
    const styledButton = useClasses(StyledToggleButton);
    const styledButtonGroup = useClasses(StyledToggleButtonGroup);

    return (
        <ToggleButtonGroup 
            classes={styledButtonGroup}
            size="small" 
            exclusive={exclusive} 
            value={value} 
            onChange={(event, value) => onChanged(value)}
        >
            {
                options && options.map((option, index) => {
                    const content = ( 
                        option.icon ? <Icon fontSize="small">{option.icon}</Icon> : <Typography text={option.label} />
                    );

                    return option.tooltip ? 
                        <TooltipToggleButton 
                            key={index}
                            classes={styledButton}
                            title={option.tooltip} 
                            value={option.value}
                            disabled={disabled ? true : option.disabled} 
                        >
                            <span>{content}</span>
                        </TooltipToggleButton> : 
                        <ToggleButton
                            key={index}
                            classes={styledButton}
                            value={option.value}
                            disabled={disabled ? true : option.disabled} 
                            aria-label={option["aria-label"] ? option["aria-label"] : option.label}
                        >
                            { content }
                        </ToggleButton>;

                })
            }
        </ToggleButtonGroup>
    );
};

export default ToggleButtons;
