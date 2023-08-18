// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {Template} from "../Template";

import {
    Button,
    Dialog,
    Menu,
    CodeEditor,
    Typography,
    useOperational,
} from "@afw/react";

import {
    isHybridScript, 
    isHybridTemplate, 
    isHybridLiteral
} from "../../../utils";

/**
 * A HybridContextualMenu prompts to get the choice from a user on whether a particular hybrid value
 * should be a Script, Template or Literal.
 */
export const HybridContextualMenu = ({ target, open, onDismiss, onSelectMenuItem }) => {
    return (
        <Menu 
            target={target}
            items={[
                {
                    key: "script",
                    label: "Adaptive Script",
                    onClick: () => onSelectMenuItem("script"),
                },
                {
                    key: "template",
                    label: "Adaptive Template",
                    onClick: () => onSelectMenuItem("template"),
                },
                {
                    key: "literal",
                    label: "Literal",
                    onClick: () => onSelectMenuItem("literal"),
                },
            ]}
            open={open}
            onClose={onDismiss}
        />
    );   
};

const RemovableHybrid = ({ children, onRemove }) => {
    
    const [warn, setWarn] = useState();
    const {editable} = useOperational();

    return (    
        <div style={{ width: "100%", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "flex-start", height: "100%" }}>
                { editable &&
                    <Button 
                        type="icon"       
                        label="Remove this Hybrid"                 
                        tooltip="Remove this Hybrid"
                        icon="close"
                        style={{ color: "red" }}
                        onClick={() => {
                            setWarn(true);
                        }}
                        disabled={editable === false}
                    />
                }
                <div style={{ flex: 1, height: "100%" }}>
                    { children }
                </div>                    
            </div>
            <Dialog
                open={Boolean(warn)}
                onDismiss={() => setWarn(false)}
                isBlocking={false}
                maxWidth="sm"
                title="Discard Changes"
                showClose={true}
                contains={
                    <Typography text="This Expression has changed.  Discard the changes?" />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: "20px", width: "100%" }}>
                        <div style={{ marginRight: "8px" }}>
                            <Button
                                label="No"
                                onClick={() => setWarn(false)}
                            />
                        </div>
                        <div>
                            <Button
                                label="Yes"
                                color="primary"
                                variant="contained"
                                onClick={() => {
                                    setWarn(false);
                                    onRemove();
                                }}
                            />
                        </div>
                    </div>
                }
            />
        </div>
    );
};

/**
 * A Hybrid represents an Adaptive Script, or a string Template.  
 *   We determine which one by whether it's a Javascript array vs string.
 *   For undefined values, we present Menu options that allow the user
 *   to choose which one to create.
 */
export const HybridBuilder = (props) => {
     
    const [contextualMenuVisible, setContextualMenuVisible] = useState(false);
    const [contextualMenuTarget, setContextualMenuTarget] = useState();
    const [hybridType, setHybridType] = useState();    

    const {editable} = useOperational(props);
    const { 
        className, 
        style, 
        hybrid, 
        dataType, 
        dataTypeParameter, 
        objectTypeObject,
        label, 
        onChanged
    } = props;   
    
    const id = (props.id ? (props.id + ".") : "") + "HybridBuilder";

    useEffect(() => {
        if (hybrid && !hybridType) {

            if (isHybridScript(hybrid))
                setHybridType("script");

            else if (isHybridTemplate(hybrid))
                setHybridType("template");

            else if (isHybridLiteral(hybrid))
                setHybridType("literal");
        }

        setContextualMenuVisible(false);
    }, [hybrid, hybridType]);

                    
    /* The hybrid hasn't been defined yet, so present Menu options */
    if (editable && !hybridType) 
    {
        return (
            <div
                id={id}
                className={className}
                style={style}
            >
                <HybridContextualMenu  
                    dataType={dataType}                        
                    dataTypeParameter={dataTypeParameter}         
                    objectTypeObject={objectTypeObject} 
                    open={contextualMenuVisible}
                    target={contextualMenuTarget}
                    onDismiss={() => setContextualMenuVisible(false)}    
                    onSelectMenuItem={hybridType => {
                        setHybridType(hybridType);
                        setContextualMenuVisible(false);
                    }}
                />
                <Button 
                    disabled={editable === false}
                    color="primary"                    
                    label={label ? label : "Hybrid"}                        
                    tooltip="Click to Add Hybrid"
                    icon="add"
                    variant="text"
                    onClick={(event) => {
                        setContextualMenuTarget(event.target);
                        setContextualMenuVisible(true);
                    }}
                />
            </div>
        );
    } 

    /* JSON Literal */
    else if (hybridType === "literal")
    {
        let literal = "";
        if ((hybrid !== undefined))
            literal = JSON.stringify(hybrid, null, 4);

        /*! \fixme getJSONSchema for object literals */

        return (
            <RemovableHybrid                 
                onRemove={() => {
                    if (onChanged)
                        onChanged();
                    setHybridType();   
                }}
            >                
                <CodeEditor 
                    id={id}
                    label={label}
                    style={{ height: "100%" }}
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={true}
                    readOnly={!editable}
                    source={literal}
                    language="json"
                    wordWrap={true}
                    showLineNumbers={true}
                    onChanged={literal => {                            
                        try {                          
                            const parsed = JSON.parse(literal);   
                            onChanged(parsed);                                
                        } catch (e) { 
                            /* not ready to be changed yet */
                        }
                    }}
                />
            </RemovableHybrid>                
        );
    }

    /* Check if it's an Adaptive Script */
    else if (hybridType === "script")
    {
        return (
            <RemovableHybrid                 
                onRemove={() => {
                    if (onChanged)
                        onChanged();
                    setHybridType();   
                }}
            >
                <CodeEditor 
                    id={id}
                    style={{ height: "100%" }}
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={true}
                    readOnly={!editable}
                    source={hybrid ? hybrid : "#! afw\n"}
                    cursorPosition={hybrid ? undefined : 7}
                    language="afw"
                    wordWrap={true}
                    showLineNumbers={true}
                    onChanged={onChanged}                                
                />
            </RemovableHybrid>                
        );
    }

    /* Template string */
    else if (hybridType === "template")
    {            
        return (
            <RemovableHybrid                 
                onRemove={() => {
                    if (onChanged)
                        onChanged();
                    setHybridType();   
                }}
            >
                <Template  
                    id={id}
                    valueMeta={{ label }}
                    value={hybrid}
                    onChanged={onChanged}
                />
            </RemovableHybrid>                
        );
    }

    else {
        /* ??? */
        return null;
    }
};

HybridBuilder.displayName = "HybridBuilder";

export default HybridBuilder;
