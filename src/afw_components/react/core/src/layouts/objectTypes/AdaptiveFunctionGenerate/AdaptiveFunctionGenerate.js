// See the 'COPYING' file in the project root for licensing information.

import {useOperational, useTheme} from "../../../hooks";
import {ObjectResponsive} from "../ObjectResponsive";

export const AdaptiveFunctionGenerate = (props) => {

    const {editable} = useOperational(props);  
    const theme = useTheme();  

    return (
        <>
            <ObjectResponsive 
                filterOptions={{
                    filterProperties: [
                        "category",
                        "brief",
                        "description",
                        "functionId",
                        "functionLabel",
                        "afwCamelCaseFunctionLabel",
                        "functionSignature",
                        "op",
                        "dataType",
                        "dataTypeMethod",
                    ],
                    filterValuedProperties: true,
                }}
                spacing={theme.spacing(1)}
            />   
            <ObjectResponsive             
                filterOptions={{
                    filterProperties: [
                        "additionalArgCheck",
                        "deprecated",
                        "polymorphic",
                        "pure",
                        "scriptSupport",
                        "signatureOnly",
                        "polymorphicExecuteFunctionEvaluatesFirstParameter"
                    ],
                    filterValuedProperties: editable ? false : true,
                }}
                showMeta={false}
                columns={editable ? 2 : 1}                 
                spacing={theme.spacing(1)}
            />                     
            <ObjectResponsive 
                filterOptions={{
                    filterProperties: [
                        "parameters"
                    ],  
                    filterValuedProperties: true,                    
                }}
                showMeta={false}
                spacing={theme.spacing(1)}
            />
            <ObjectResponsive 
                filterOptions={{
                    filterProperties: [
                        "errorsThrown",
                        "sideEffects"
                    ],
                    filterValuedProperties: true
                }}                  
                showMeta={false}
                spacing={theme.spacing(1)}
            />
            <ObjectResponsive 
                filterOptions={{
                    filterProperties: [
                        "returns"
                    ],
                    filterValuedProperties: true
                }}
                embedded={{
                    component: "Box"
                }}            
                showMeta={false}       
                spacing={theme.spacing(1)}  
            />
        </>
    );
};

export default AdaptiveFunctionGenerate;
