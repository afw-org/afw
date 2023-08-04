// See the 'COPYING' file in the project root for licensing information.
import FormControl from "@mui/material/FormControl";
import FormLabel from "@mui/material/FormLabel";
import FormControlLabel from "@mui/material/FormControlLabel";
import FormHelperText from "@mui/material/FormHelperText";
import RadioGroup from "@mui/material/RadioGroup";
import Radio from "@mui/material/Radio";

export const ChoiceGroup = ({ 
    label, "aria-label": ariaLabel, orientation, onChanged, selectedKey, options, 
    disabled, description, id, "data-testid": dataTestId, "data-component-type": dataComponentType 
}) => 
    <FormControl 
        disabled={disabled}
        id={id}
        data-testid={dataTestId}
        data-component-type={dataComponentType}
    >
        {
            label && 
                    <FormLabel>{label}</FormLabel>                                              
        }
        <RadioGroup 
            name={label}
            aria-label={ariaLabel}
            row={orientation === "horizontal" ? true : false}
            onChange={(event, value) => {
                if (onChanged)
                    onChanged({ key: value });
            }}
            value={selectedKey}                                
        >
            {
                options && options.map((option, index) => 
                    <FormControlLabel 
                        key={index} 
                        value={option.key} 
                        control={<Radio />} 
                        label={option.text} 
                        disabled={option.disabled} 
                    />
                )
            }
        </RadioGroup>      
        {
            description &&
                <FormHelperText>{description}</FormHelperText>
        }
    </FormControl>;

export default ChoiceGroup;
