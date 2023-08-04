// See the 'COPYING' file in the project root for licensing information.
import MuiTextField from "@mui/material/TextField";

export const TextField = ({ 
    id, "data-testid": dataTestId, "data-component-type": dataComponentType,
    "aria-label": ariaLabel, label, description, dataType, value, disabled, 
    multiline, rows, rowsMax, required, placeholder, onEnter, error,
    onBlur, autoFocus, onChanged, type
}) => {
        
    return (
        <MuiTextField    
            id={id}            
            data-testid={dataTestId}
            data-component-type={dataComponentType}            
            label={label}
            helperText={description}
            onChange={(event) => onChanged ? onChanged(event.target.value) : undefined}
            type={type ? type : (dataType === "integer") ? "number": "string"}
            value={(value !== undefined) ? String(value) : ""}
            disabled={disabled}
            multiline={multiline}
            rows={rows}
            maxRows={rowsMax}
            required={required}
            placeholder={placeholder}
            fullWidth={true}
            onKeyDown={(evt) => {
                if (onEnter && evt.keyCode === 13) 
                    onEnter(evt);
            }}
            onBlur={onBlur}
            // eslint-disable-next-line jsx-a11y/no-autofocus
            autoFocus={autoFocus}        
            inputProps={{
                "aria-label": ariaLabel
            }}        
            variant="standard"
            error={error}
        />
    );    
};

export default TextField;
