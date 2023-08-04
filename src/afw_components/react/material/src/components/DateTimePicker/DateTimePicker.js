// See the 'COPYING' file in the project root for licensing information.
import LocalizationProvider from "@mui/lab/LocalizationProvider";
import AdapterMoment from "@mui/lab/AdapterMoment";
import MuiDateTimePicker from "@mui/lab/DateTimePicker";
import TextField from "@mui/material/TextField";

export const DateTimePicker = ({ value, label, id, description, onChanged }) => {
    let date;
    if (value) {
        const parsed = Date.parse(value);
        if (typeof parsed == "number")
            date = new Date(Date.parse(value));        
    }
    
    return (
        <LocalizationProvider dateAdapter={AdapterMoment}>
            <MuiDateTimePicker 
                label={label}
                id={id}
                value={date}
                onChange={(date) => {                        
                    onChanged(date.format("YYYY-MM-DDTHH:mm:ssZ"));
                }}
                clearable
                inputFormat="MM/DD/YYYY hh:mm a"                
                helperText={description}
                fullWidth
                renderInput={(params) => <TextField {...params} id={id} variant="standard" helperText={description} />}
            />
        </LocalizationProvider>
    );
};

export default DateTimePicker;
