// See the 'COPYING' file in the project root for licensing information.
import moment from "moment";

import {LocalizationProvider} from "@mui/x-date-pickers/LocalizationProvider";
import {AdapterMoment} from "@mui/x-date-pickers/AdapterMoment";
import {DatePicker as MuiTimePicker} from "@mui/x-date-pickers/DatePicker";

export const TimePicker = ({ value, label, id, description, onChanged }) => {
    let date;
    if (value) {
        const parsed = Date.parse(value);
        if (typeof parsed == "number")
            date = moment(parsed);
    }

    return (
        <LocalizationProvider
            dateAdapter={AdapterMoment}
        >
            <MuiTimePicker
                label={label}
                value={date}
                onChange={(date) => {
                    if (date)
                        onChanged(date.toISOString());
                }}
                slotProps={{
                    textField: {
                        id,
                        variant: "standard",
                        helperText: description,
                        fullWidth: true
                    }
                }}
            />
        </LocalizationProvider>
    );
};

export default TimePicker;
