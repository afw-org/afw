// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Hybrid
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {useState, useMemo} from "react";

import {
    Button,
    Typography,
    QualifiersProvider,    
    useOperational,
} from "@afw/react";

import {HybridEditor} from "./HybridEditor";
import {isHybridScript, isHybridTuple, isHybridLiteral} from "../../../utils";

/**
 * Handles dataType=hybrid
 */
export const Hybrid = (props) => {

    const [expanded, setExpanded] = useState(false);
    const {editable} = useOperational(props);    

    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate, property} = props;    
    const {contextType} = valueMeta;    

    const [representedValue, code] = useMemo(() => {        

        if (value) {
            if (isHybridScript(value)) {
                return [ "<Adaptive Script>", true ];                
            }

            else if (isHybridTuple(value)) {
                return [ "<Expression Tuple>", true ];
            }

            else if (isHybridLiteral(value))
                return [ JSON.stringify(value, null, 4), false ];                

            else
                return [ value.substring(0, 64), false ];
        } else 
            return [ "", false ];

    }, [value]);

    const contextTypes = useMemo(() => {
        if (contextType)
            return [ contextType ];
    }, [contextType]);


    let objectTypeObject;
    if (property?.getDataTypeParameter()?.indexOf("object ") === 0) {
        const objectTypeId = property.getDataTypeParameter().split(" ")[1];

        if (objectTypeId) {
            const model = property.getEmbeddingObject()?.getModel();
            const adaptorId = property.getEmbeddingObject()?.getAdaptorId();

            objectTypeObject = model.getObjectTypeObject({ adaptorId, objectTypeId });
        }        
    }
    
    if (editable)
        return (
            <QualifiersProvider contextTypes={contextTypes}>
                <HybridEditor 
                    id={id}
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={autoFocus}
                    {...valueMeta} 
                    value={value} 
                    objectTypeObject={objectTypeObject}
                    onChanged={onChanged} 
                    onValidate={onValidate} 
                    onDismiss={() => {                        
                        //! \fixme not sure if we need to do anything                        
                    }}                    
                />
            </QualifiersProvider>
        );
    
    else
        return (
            <>
                <Typography 
                    style={{ 
                        display: "inline-block", 
                        whiteSpace: (expanded && code) ? "pre-wrap" : "normal", 
                        fontFamily: (expanded && code) ? "monospace" : undefined 
                    }} 
                    text={(!expanded) ? representedValue : value} 
                    size={(code && expanded) ? "2" : "5"}
                />
                {
                    (!expanded && value && value.length > 64) &&
                        <Button 
                            style={{ display: "inline-block" }} 
                            type="icon"
                            label="Expand Details"
                            icon="more_horizontal"
                            onClick={() => setExpanded(true)}
                        />
                }
            </>
        );

};

Hybrid.displayName = "Hybrid";

export default Hybrid;
