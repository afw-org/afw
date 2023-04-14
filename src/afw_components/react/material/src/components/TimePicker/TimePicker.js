// See the 'COPYING' file in the project root for licensing information.
import LocalizationProvider from "@mui/lab/LocalizationProvider";
import AdapterMoment from "@mui/lab/AdapterMoment";
import MuiTimePicker from "@mui/lab/DatePicker";
import TextField from "@mui/material/TextField";

export const TimePicker = ({ value, label, id, description, onChanged }) => {
    let date;
    if (value) {
        const parsed = Date.parse(value);
        if (typeof parsed == "number")
            date = new Date(parsed);
    }
    
    return (
        <LocalizationProvider                 
            dateAdapter={AdapterMoment}
        >
            <MuiTimePicker 
                label={label}                
                value={date}
                onChange={(date) => {
                    onChanged(date.toISOString());
                }}
                mask="__:__ _M"
                fullWidth
                renderInput={(params) => <TextField {...params} id={id} variant="standard" helperText={description} />}
            />
        </LocalizationProvider>
    );
};

export default TimePicker;
