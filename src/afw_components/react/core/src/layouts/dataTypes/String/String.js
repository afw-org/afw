// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for String
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState} from "react";

import {
    Button,
    CodeEditor,
    TextField, 
    Typography,
    useOperational
} from "@afw/react";

import {ExpandableComponent} from "../../../utils";
import propTypes from "../dataTypes.propTypes";

/**
 * StringReadOnly
 * 
 * Handles dataType=string in editable=false mode.
 */
const StringReadOnly = (props) => {

    const [expanded, setExpanded] = useState(false);

    const {"aria-label": ariaLabel, value, multiline} = props;

    // max number of characters before it makes ellipses
    const maxChars = 256;
    
    if (value && value.length > maxChars) {
        return (
            <div style={{ display: "flex" }}>
                <Typography
                    style={{ whiteSpace: multiline ? "pre-wrap" : "normal" }}
                    text={!expanded ? value.substring(0, maxChars) : value}
                    component="span"
                    aria-label={ariaLabel}
                />
                {
                    (!expanded && value && value.length > maxChars) &&
                        <Button
                            type="icon"
                            label="Expand Details"
                            icon="more_horizontal"
                            onClick={() => setExpanded(true)}
                        />
                }
            </div>
        );
    }

    else {
        return (
            <Typography 
                style={{ whiteSpace: multiline ? "pre-wrap" : "normal" }}
                aria-label={ariaLabel}
                size="5"
                component="span"
                text={value || ""}
            />
        );
    }
};

/**
 * Plaintext
 * 
 * This handles dataType=string, dataTypeParameter=text/plain
 * 
 * Uses an expandable component to display the string value inside 
 * a large CodeEditor for easier editing of larger content.
 */
const Plaintext = (props) => {

    const {id, value, onChanged, valueMeta, autoFocus, onValidate, brief, description} = props;

    const {editable} = useOperational(props);

    // seems like a reasonable default
    const rows = 4;

    if (editable) {
        return (
            <ExpandableComponent
                expandedChildren={
                    <CodeEditor 
                        style={{ flex: 1, height: "100%" }}
                        id={id}
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        readOnly={!editable}
                        source={value}
                        language="text/plain"
                        wordWrap={true}
                        showMinimap={true}
                        onChanged={onChanged}
                    />
                }
            >
                <TextField 
                    id={id}                
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={autoFocus}
                    {...valueMeta}
                    description={brief || description}
                    type="text"
                    value={value}
                    multiline={true}         
                    rows={rows}          
                    onChanged={onChanged}                                                      
                    onValidate={onValidate}
                />   
            </ExpandableComponent>
        );
    } else {
        return (
            <StringReadOnly {...props} value={value.replaceAll("\\n", "\n")} multiline />
        );
    }
};

/**
 * _String
 * 
 * Handles dataType=string 
 */
export const _String = (props) => {
    
    const {editable} = useOperational(props);

    const {id, value, valueMeta = {}, onChanged, onValidate, autoFocus} = props;
    const {dataTypeParameter: mediaType, brief, description} = valueMeta;

    if (mediaType === "text/plain") {
        return (
            <Plaintext {...props} />
        );
    }

    if (editable) {                        
        return (
            <TextField 
                id={id}                
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta}
                description={brief || description}
                type="text"
                value={value}          
                onChanged={onChanged}                                                      
                onValidate={onValidate}
            />        
        );
    } else {
        return (
            <StringReadOnly {...props} />
        );
    }
};

_String.propTypes = propTypes;

export default _String;
