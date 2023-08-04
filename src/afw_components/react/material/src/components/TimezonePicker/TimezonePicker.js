// See the 'COPYING' file in the project root for licensing information.
import {Component} from "react";
import PropTypes from "prop-types";

import moment from "moment-timezone";

import Autocomplete from "../Autocomplete";

export class TimezonePicker extends Component {

    state = {           
        timezoneOptions: [],   
    };

    componentDidMount() {
        const now = moment().unix();
        let timezoneOptions = moment.tz.names().map(name => {
            const tz = moment(now).tz(name);

            return ({
                key: name,
                text: "(GMT" + tz.format("Z") + ") " + name,
                offset: tz.format("Z"),
                abbr: tz.zoneAbbr(),
            });
        });
        
        timezoneOptions = timezoneOptions.sort((tz1, tz2) => {
            if (tz1.offset.startsWith("-")) {
                if (tz2.offset.startsWith("-"))
                    return -tz1.offset.localeCompare(tz2.offset);
                else
                    return -1;
            } else if (tz2.offset.startsWith("-")) {
                return 1;
            } else 
                return tz1.offset.localeCompare(tz2.offset);
        });
        
        this.setState({ timezoneOptions });

        if (this.props.value)            
            this.parseOffset(this.props.value, timezoneOptions);        
    }

    componentDidUpdate(prevProps) {
        if (prevProps.value !== this.props.value) {
            this.parseOffset(this.props.value, this.state.timezoneOptions);
        }
    }

    parseOffset = (offset, timezoneOptions) => {
        if (offset) {            
            let selectedTimezoneOption;                           
           
            for (let timezoneOption of timezoneOptions) {
                if (timezoneOption.offset === offset) {
                    selectedTimezoneOption = timezoneOption;
                    break;
                }
            }        

            this.setState({ selectedTimezoneOption });
        }
    };

    onChanged = (selectedTimezoneOption) => {
        this.setState({ selectedTimezoneOption });

        this.props.onChanged(selectedTimezoneOption.offset);
    };

    render() {
        return (
            <Autocomplete          
                id={this.props.id}
                className={this.props.className}
                style={this.props.style}                   
                placeholder="Timezone"
                options={this.state.timezoneOptions}
                value={this.state.selectedTimezoneOption}
                onChanged={this.onChanged}
            />
        );
    }
}

TimezonePicker.propTypes = {
    "aria-label":          PropTypes.string,
    description:        PropTypes.string,
    disabled:           PropTypes.bool,
    label:              PropTypes.string,
    required:           PropTypes.bool,
    value:              PropTypes.string,
};

export default TimezonePicker;
