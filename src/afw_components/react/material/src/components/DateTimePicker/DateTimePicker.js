// See the 'COPYING' file in the project root for licensing information.
import moment from "moment";

import {LocalizationProvider} from "@mui/x-date-pickers/LocalizationProvider";
import {AdapterMoment} from "@mui/x-date-pickers/AdapterMoment";
import {DateTimePicker as MuiDateTimePicker} from "@mui/x-date-pickers/DateTimePicker";

export const DateTimePicker = ({ value, label, id, description, onChanged }) => {
    let date;
    if (value) {
        const parsed = Date.parse(value);
        if (typeof parsed == "number")
            date = moment(parsed);
    }

    return (
        <LocalizationProvider dateAdapter={AdapterMoment}>
            <MuiDateTimePicker
                label={label}
                value={date}
                onChange={(date) => {
                    if (date)
                        onChanged(date.format("YYYY-MM-DDTHH:mm:ssZ"));
                }}
                format="MM/DD/YYYY hh:mm a"
                slotProps={{
                    field: { clearable: true },
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

export default DateTimePicker;
