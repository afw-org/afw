// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import Select from "@mui/material/Select";
import MenuItem from "@mui/material/MenuItem";
import FormControl from "@mui/material/FormControl";
import FormHelperText from "@mui/material/FormHelperText";
import InputLabel from "@mui/material/InputLabel";
import ListItemText from "@mui/material/ListItemText";
import Divider from "@mui/material/Divider";
import Typography from "@mui/material/Typography";

import Icon from "../Icon";


export const Dropdown = (props) => {

    const {id, "data-testid": dataTestid, options, label, description, disabled, placeholder, required, className, style} = props;

    const onChange = (event) => {
        let key = event.target.value;
        options.forEach((option) => {
            if (option.key === key) {
                if (props.onChanged)
                    props.onChanged(option);
            }
        });
    };
   
    let selectedKey;
    if (props.selectedKeys && props.selectedKeys[0])
        selectedKey = props.selectedKeys[0];
    else if (props.selectedKey)
        selectedKey = props.selectedKey;
    else
        selectedKey = "";

    const dropdownOptions = useMemo(() => {
        if (options) {
            return options.map((option, index) => {
                if (option.type === "header") {
                    return <Typography style={{ lineHeight: "32px", paddingLeft: "8px" }} color="secondary" variant="body2" key={index} >{ option.text }</Typography>;
                }

                else if (option.type === "divider") {
                    return <Divider key={index} />;
                }
                    
                return (
                    <MenuItem key={option.key ? option.key : index} value={option.key}>
                        {
                            option.icon && 
                                <Icon iconName={option.icon} color="primary" style={{ display: "inline-block", verticalAlign: "top", paddingRight: "8px" }} />

                        }
                        <ListItemText style={{ display: "inline-block" }} primary={option.text} secondary={option.description} />
                    </MenuItem>
                );
            });
        } else return null;
    }, [options]);

    return (
        <FormControl
            className={className}
            style={style}  
            disabled={disabled}      
            required={required}             
            fullWidth
            variant="standard"
        >
            {
                label &&
                    <InputLabel id={id ? (id + "-label") : undefined}>{label}</InputLabel>
            }
            <Select 
                id={id}
                data-testid={dataTestid}
                labelId={id ? (id + "-label") : undefined}                
                value={(options && options.length) ? selectedKey : ""}
                onChange={onChange}
                placeholder={placeholder}
                variant="standard"    
            >
                { dropdownOptions }
            </Select>
            {
                description &&
                    <FormHelperText>{description}</FormHelperText>
            }
        </FormControl>
    );    
};

export default Dropdown;
