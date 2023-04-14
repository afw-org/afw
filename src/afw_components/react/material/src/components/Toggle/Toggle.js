// See the 'COPYING' file in the project root for licensing information.
import FormControlLabel from "@mui/material/FormControlLabel";
import Switch from "@mui/material/Switch";

export const Toggle = ({ value, label, onChanged, placement }) => {
    const onChange = (event) => {
        if (onChanged)
            onChanged(event.target.checked);
    };

    return (            
        <FormControlLabel                        
            control={
                <Switch 
                    checked={value}
                    onChange={onChange}
                />
            }
            label={label}
            labelPlacement={placement}
        />            
    );
};

export default Toggle;
