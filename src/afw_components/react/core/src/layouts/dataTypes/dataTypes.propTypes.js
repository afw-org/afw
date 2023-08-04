// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

/* derived from _AdaptiveValueMeta_ */
const ValueMeta = {     
    additionalConstraints:      PropTypes.string,
    allowedValues:              PropTypes.array,
    allowWrite:                 PropTypes.bool,
    brief:                      PropTypes.string,
    contextType:                PropTypes.string,
    dataType:                   PropTypes.string,
    dataTypeParameter:          PropTypes.string,
    defaultValue:               PropTypes.any,
    description:                PropTypes.string,
    hints:                      PropTypes.object,
    label:                      PropTypes.string,
    maxLength:                  PropTypes.number,
    maxNormalLength:            PropTypes.number,
    maxValue:                   PropTypes.any,
    minLength:                  PropTypes.number,
    minValue:                   PropTypes.any,
    possibleValues:             PropTypes.array,
    required:                   PropTypes.bool,
    testDataParameter:          PropTypes.string,
    unique:                     PropTypes.bool,  
};

export const propTypes = {     
    valueMeta:                  PropTypes.shape(ValueMeta),
    value:                      PropTypes.any,
    onChanged:                  PropTypes.func,    
    onValidate:                 PropTypes.func,
};

export default propTypes;
