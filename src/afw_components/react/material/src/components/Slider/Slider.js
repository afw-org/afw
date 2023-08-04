// See the 'COPYING' file in the project root for licensing information.
import MuiSlider from "@mui/material/Slider";
import Typography from "@mui/material/Typography";

export const Slider = ({ 
    disabled, min, max, value, step, onChanged, vertical, label, description, marks
}) => {
    
    if (value && !max)
        max = value * 2;

    return (
        <>
            { label && <Typography>{label}</Typography> }
            <MuiSlider             
                aria-label={label}                       
                disabled={disabled}
                max={max}
                min={min}                                                
                step={step}
                onChange={(event, value) => {                    
                    if (onChanged)
                        onChanged(value);
                }}
                value={value}
                orientation={vertical ? "vertical": "horizontal"}
                marks={marks}
            />            
            { description && <Typography variant="caption">{description}</Typography> }
        </>
    );    
};

export default Slider;
