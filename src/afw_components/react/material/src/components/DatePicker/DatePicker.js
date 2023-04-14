// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";
import moment from "moment";

import LocalizationProvider from "@mui/lab/LocalizationProvider";
import AdapterMoment from "@mui/lab/AdapterMoment";
import MuiDatePicker from "@mui/lab/DatePicker";
import TextField from "@mui/material/TextField";

import TimezonePicker from "../TimezonePicker";

export const DatePicker = (props) => {

    const [momentDate, setMomentDate] = useState(moment());
    const [offset, setOffset] = useState();

    const value = props.value;

    useEffect(() => {
        if (value) {
            const momentDate = moment(value, "YYYY-MM-DDZ");
            const offset = momentDate.format("Z");   
     
            setMomentDate(momentDate);
            setOffset(offset);  
        }
    }, [value]);


    const onChanged = (momentDate) => {
        setMomentDate(momentDate);
        if (props.onChanged)
            props.onChanged( momentDate.format("YYYY-MM-DDZ") );
    };

    const onDateChanged = (value) => {        
        const offset = momentDate.utcOffset();
        value.utcOffset(offset);        
        onChanged(value);
    };

    const onTimezoneChanged = (offset) => {
        momentDate.utcOffset(offset);
        onChanged( momentDate );
    };

    const date = momentDate; 

    // \fixme the MuiDatePicker seems to set the date relative to the local timezone?
    return (
        <LocalizationProvider dateAdapter={AdapterMoment}>            
            <div style={{ display: "flex" }}>
                <div style={{ marginRight: "8px" }}>
                    <MuiDatePicker 
                        id={props.id}
                        label={props.label}
                        aria-label={props["aria-label"]}
                        value={date.toDate()}
                        onChange={onDateChanged}
                        format="YYYY-MM-DD"
                        helperText={props.description}
                        fullWidth                        
                        renderInput={(params) => <TextField {...params} id={props.id} variant="standard" helperText={props.description} />}
                    />
                </div>
                <div style={{ flex: 1 }}>
                    <TimezonePicker
                        id={props.id ? (props.id + "-tz") : undefined}
                        value={offset} 
                        onChanged={onTimezoneChanged}
                    />
                </div>
            </div>
        </LocalizationProvider>
    );
};

export default DatePicker;
