// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import PropTypes from "prop-types";

import {Button} from "@afw/react";
import {ExpressionTupleOperations} from "./ExpressionTupleOperations";
import {TupleContextualMenu} from "./TupleContextualMenu";

import {useOperational} from "@afw/react";

/*
 * An ExpressionTuple is an encoded expression of the form:
 * 
 *   ExpressionTuple ::=
 *      '[' ExpressionTupleAnnotation ',' ExpressionTupleOperation ']'
 * 
 * The annotation is any UI-defined (or well-defined by _AdaptiveAnnotation_) 
 *   properties that may determine how to best render this particular expression.
 * 
 * The operation may be a function, or one of the built-in expression operations.
 *  
 */
export const ExpressionTuple = (props) => {

    const [contextualMenuTarget, setContextualMenuTarget] = useState();
    
    const onTupleChanged = (tuple) => {
        setContextualMenuTarget();
        
        props.onChanged(tuple);
    };
 
    const {tuple, label, onChanged, dataType, dataTypeParameter} = props;
    const {editable} = useOperational(props);
            
    if (!tuple || tuple.length === 0) {        
        if (editable)
            return (
                <>
                    <TupleContextualMenu 
                        allowLiterals={false}
                        allowTuples={true}
                        visible={Boolean(contextualMenuTarget)}
                        target={contextualMenuTarget}
                        onDismiss={() => setContextualMenuTarget()}        
                        onChanged={onTupleChanged}                
                    />
                    <Button
                        label={label ? label : "Expression"}
                        color="primary"
                        variant="text"
                        icon="add"
                        size="small"
                        onClick={(event) => setContextualMenuTarget(event.target)}
                        disabled={editable === false}
                    />
                </>
            );
        else return null;
    }

    const operation = ExpressionTupleOperations[tuple[1]];
    if (operation) {
        const ExpressionTupleOperation = operation.component;
        return (
            <ExpressionTupleOperation 
                tuple={tuple}
                onChanged={onChanged}
                dataType={dataType}
                dataTypeParameter={dataTypeParameter}
                label={operation.label}
                description={operation.description}
            />
        );
    } else {
        /*! \fixme this may be a direct adaptive function call */
        throw new Error("Invalid Expression Tuple Operation: " + tuple[1]);        
    }       
};

ExpressionTuple.propTypes = {
    allowWrite:         PropTypes.bool,
    tuple:              PropTypes.array,
};

export default ExpressionTuple;
