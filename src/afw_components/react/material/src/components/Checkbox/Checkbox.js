// See the 'COPYING' file in the project root for licensing information.
import FormControl from "@mui/material/FormControl";
import FormHelperText from "@mui/material/FormHelperText";
import FormControlLabel from "@mui/material/FormControlLabel";
import MuiCheckbox from "@mui/material/Checkbox";
import Icon from "@mui/material/Icon";
import Tooltip from "@mui/material/Tooltip";

export const Checkbox = ({ 
    disabled, required, icon, checkedIcon, value, onChanged, label, "aria-label": ariaLabel, style,
    description, id, "data-testid": dataTestId, "data-component-type": dataComponentType, tooltip, 
}) => {

    let control = (
        <MuiCheckbox 
            icon={icon ? <Icon>{icon}</Icon> : undefined}
            checkedIcon={checkedIcon ? <Icon>{checkedIcon}</Icon> : undefined}
            value={value ? String(value) : "false"}                                
            checked={value ? true : false}
            onChange={(event, checked) => onChanged ? onChanged(checked) : undefined}
            disabled={disabled}
            inputProps={ ariaLabel ? { "aria-label": ariaLabel, } : undefined}
        />
    );

    if (tooltip)
        control = (
            <Tooltip title={tooltip}>
                { control }
            </Tooltip>
        );

    return (
        <FormControl 
            id={id}
            style={style}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            disabled={disabled}
            required={required}
        >
            { 
                label ?
                    <FormControlLabel
                        control={
                            <MuiCheckbox 
                                icon={icon ? <Icon>{icon}</Icon> : undefined}
                                checkedIcon={checkedIcon ? <Icon>{checkedIcon}</Icon> : undefined}
                                value={value ? String(value) : "false"}                                
                                checked={value ? true : false}
                                onChange={(event, checked) => onChanged ? onChanged(checked) : undefined}
                                disabled={disabled}
                                inputProps={ ariaLabel ? { "aria-label": ariaLabel, } : undefined}
                            />
                        }
                        label={label}
                    /> : control
            }
            {
                description &&
                    <FormHelperText sx={{ marginLeft: 0 }}>{description}</FormHelperText>
            }
        </FormControl>
    );
};

export default Checkbox;
