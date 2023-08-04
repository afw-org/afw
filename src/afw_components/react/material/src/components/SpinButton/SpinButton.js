// See the 'COPYING' file in the project root for licensing information.
import TextField from "@mui/material/TextField";

export const SpinButton = ({ 
    label, description, placeholder, min, max, step, value, disabled, required, onChanged, onBlur, autoFocus
}) => {
    return (
        <TextField         
            type="number"    
            variant="standard"                            
            label={label}
            helperText={description}
            onChange={(event) => {
                if (onChanged) {
                    onChanged(event.target.value);
                }
            }}
            value={value}
            disabled={disabled}                
            required={required}
            placeholder={placeholder}
            fullWidth={true}
            onBlur={onBlur}
            // eslint-disable-next-line jsx-a11y/no-autofocus
            autoFocus={autoFocus}           
            min={min}     
            max={max}
            step={step}
        />
    );    
};

export default SpinButton;
