// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";
import PropTypes from "prop-types";

import {
    Autocomplete,
    Button,
    Typography
} from "@afw/react";
import {useQualifiers, useOperational} from "@afw/react";

/*
 * ExpressionTupleOperationVariable
 *
 * This component represents selecting a variable inside an expression builder.
 * These variables are derived from the qualifiers that are provided via the 
 * useQualifiers hook.  Different providers can override/add qualifiers depending
 * on the context that the expression is being designed for.  
 * 
 * For example, expressions for an authorization policy will require different 
 * variables than say an expression for a log message.
 */
const ExpressionTupleOperationVariable = (props) => {

    const [autocompleteOptions, setAutocompleteOptions] = useState([]);
    const [/*editing*/, setEditing] = useState();

    const qualifiers = useQualifiers();
    const {editable} = useOperational(props);
    
    useEffect(() => {
        // \fixme not using adaptive objects here
        if (qualifiers) {
            let autocompleteOptions = [];

            for (const {qualifierName, qualifierDef} of qualifiers) {
                /* get and sort the variable names */                
                const variables = qualifierDef;

                if (!variables)
                    continue;

                const sortedVariables = Object.entries(variables).map(([varName, ]) => 
                    qualifierName + "::" + varName
                ).sort((a, b) => a.toLowerCase().localeCompare(b.toLowerCase()));

                sortedVariables.forEach(variable => {
                    autocompleteOptions.push({
                        key: variable,
                        text: variable,
                    });
                });               
            }   

            setAutocompleteOptions(autocompleteOptions);
        }
    }, [qualifiers]);

    let component;

    if (editable) {
        component = 
            <Autocomplete 
                label="Variable Name"
                description="Select a variable to reference."
                options={autocompleteOptions}
                value={{
                    key: props.tuple[2],
                    text: props.tuple[2]
                }}
                onChanged={(option) => {
                    let tuple = props.tuple;
                    if (option)
                        tuple[2] = option.key;
                    props.onChanged(tuple);
                    setEditing(false);
                }}
                onCreateOption={value => {
                    let tuple = props.tuple;
                    let newOption = {
                        key: value,
                        text: value,
                    };

                    tuple[2] = value;
                    props.onChanged(tuple);

                    const options = [ ...autocompleteOptions, newOption ];
                    
                    setAutocompleteOptions(options);
                    setEditing(false);
                }}
                noOptionsText="No Variable"
            />;
    } else {
        component = 
            <div 
                style={{ display: "flex" }}
                onClick={editable === false ? undefined : () => setEditing(true)}
                onKeyPress={editable === false ? undefined : () => setEditing(true)}
                role="button"
                tabIndex="0"                
            >
                <Typography color="textSecondary" text="Variable(" />
                <Typography text={props.tuple[2] ? props.tuple[2] : "Select a variable..."} />
                <Typography color="textSecondary" text=")" />
            </div>;
    }

    if (editable)
        return (
            <div style={{ display: "flex" }}>
                <div>
                    <Button 
                        type="icon"
                        label="Remove Variable"
                        style={{ color: "#f44336" }}
                        icon="close"
                        onClick={() => {
                            props.onChanged();
                        }}
                    />                
                </div>                
                <div style={{ flex: 1 }}> 
                    {component}
                </div>
            </div>
        );
    else return component;
};

ExpressionTupleOperationVariable.propTypes = {
    tuple:                  PropTypes.array,
    onChanged:              PropTypes.func,
};

export default ExpressionTupleOperationVariable;
