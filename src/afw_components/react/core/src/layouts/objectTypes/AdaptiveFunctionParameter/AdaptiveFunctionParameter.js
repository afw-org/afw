// See the 'COPYING' file in the project root for licensing information.
import {useOperational} from "../../../hooks";
import {ObjectResponsive} from "../ObjectResponsive";

export const AdaptiveFunctionParameter = (props) => { 

    const {editable} = useOperational(props);

    return (
        <ObjectResponsive 
            filterOptions={{
                filterProperties: [
                    "name",
                    "brief",
                    "description",
                    "dataType",
                    "dataTypeParameter",
                    "minArgs",
                    "optional",
                    "polymorphicDataType",
                    "polymorphicDataTypeParameter"
                ],
                filterValuedProperties: editable ? false : true
            }}
        />
    );
};

export default AdaptiveFunctionParameter;
