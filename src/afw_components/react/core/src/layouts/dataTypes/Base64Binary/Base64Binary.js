// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Base64Binary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {useState} from "react";

import {
    Button,
    TextField,
    Typography,
    useOperational
} from "@afw/react";


/**
 * Handles dataType=base64Binary, dataTypeParameter=image/* 
 */
const Base64BinaryImage = (props) => {

    const {editable} = useOperational(props);
    const {value, label, brief, valueMeta} = props;    

    const {dataTypeParameter : mediaType} = valueMeta;

    if (value !== undefined) {
        if (editable) {
            return (
                <>
                    <Typography 
                        size="5"
                        color="textSecondary"
                        text={label}
                    />
                    <img 
                        alt={label} 
                        src={"data:" + mediaType + ";base64," + value} 
                    />
                    <Typography 
                        size="1"                                
                        text={brief ? brief : ""}
                    />
                </>
            );

        } else {
            return (
                <img 
                    alt={label} 
                    src={"data:" + mediaType + ";base64," + value} 
                />
            );
        }
    }
};

/**
 * Handles dataType=base64Binary
 */
export const Base64Binary = (props) => {

    const [expanded, setExpanded] = useState(false);
    const {editable} = useOperational(props);

    const {id, autoFocus, "data-testid": dataTestId, value, valueMeta = {}, onChanged, onValidate} = props;
    
    const {dataTypeParameter: mediaType, label, brief, description} = valueMeta;

    if (mediaType && mediaType.startsWith("image/")) {
        return <Base64BinaryImage {...props} />;        
    }
    
    if (editable)
        return (
            <TextField 
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus                
                autoFocus={autoFocus}
                label={label}
                data-testid={dataTestId}
                {...valueMeta}
                type="text"
                description={brief ? brief : description}                
                onValidate={onValidate}
                multiline={true}
                rowsMax={4}
                value={value ? atob(value) : value}
                onChanged={value => onChanged(btoa(value))}
            />
        );

    else 
    {
        // seems like a reasonable max to show
        const maxChars = 32;

        return(
            <>
                <Typography
                    id={id}
                    data-testid={dataTestId}
                    style={{
                        width: "100%",
                        display: "inline-block",
                        overflowWrap: "break-word",
                        wordWrap: "break-word",
                    }}
                    text={(!expanded && value) ? value.substring(0, maxChars) : value}
                />
                {
                    (!expanded && value && value.length > maxChars) &&
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
    }
};

Base64Binary.displayName = "Base64Binary";

export default Base64Binary;
