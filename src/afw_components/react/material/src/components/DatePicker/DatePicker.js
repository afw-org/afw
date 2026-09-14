// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";
import moment from "moment";

import {LocalizationProvider} from "@mui/x-date-pickers/LocalizationProvider";
import {AdapterMoment} from "@mui/x-date-pickers/AdapterMoment";
import {DatePicker as MuiDatePicker} from "@mui/x-date-pickers/DatePicker";

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
        if (!value)
            return;

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
                        label={props.label}
                        aria-label={props["aria-label"]}
                        value={date}
                        onChange={onDateChanged}
                        format="YYYY-MM-DD"
                        slotProps={{
                            textField: {
                                id: props.id,
                                variant: "standard",
                                helperText: props.description,
                                fullWidth: true
                            }
                        }}
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
