// See the 'COPYING' file in the project root for licensing information.
import {createContext, useContext, useLayoutEffect, useRef, useState} from "react";
import PropTypes from "prop-types";

import {
    Button,
    Callout,    
    Collapsible,
    Dialog,    
    EditableCallout,
    Menu,    
    Responsive,    
    Typography,
    useTheme,
} from "@afw/react";

import {useOperational, useClipboard} from "@afw/react";

const ExpressionGroupContext = createContext();

/**
 * ExpressionTupleGroupHeader
 * 
 * This component is responsible for the "header" on each ExpressionTupleGroup, which 
 * provides icon buttons for showing help, adding annotation, and the ability to copy
 * and paste tuples.
 */
export const ExpressionTupleGroupHeader = (props) => {

    const [showInfoCallout, setShowInfoCallout] = useState(false);
    const [showAnnotationCallout, setShowAnnotationCallout] = useState(false);
    const [menu, setMenu] = useState(false);
    const [menuTarget, setMenuTarget] = useState(false);
    const [infoRef, setInfoRef] = useState();
    const [annotationRef, setAnnotationRef] = useState();

    const {clipboard, onCopy: onClipboardCopy} = useClipboard();
    const {editable} = useOperational(props);
    const theme = useTheme();

    const showAnnotation = (event) => {
        event.stopPropagation();

        setShowAnnotationCallout(true);
    };

    const showHelp = (event) => {
        event.stopPropagation();

        setShowInfoCallout(true);
    };

    const toggleExpand = (event) => {
        event.stopPropagation();
        const expanded = !props.expanded;
        
        props.onToggleExpand(expanded);
    };

    const toggleMenu = (event) => {
        event.stopPropagation();

        setMenu(!menu);
        setMenuTarget(!menu ? event.currentTarget : undefined);
    };

    const onCopy = (event) => {
        event.stopPropagation();

        onClipboardCopy(props.tuple);

        setMenu(false);
        setMenuTarget();
    };

    const onPaste = (event) => {
        event.stopPropagation();

        let tuple = clipboard;
        if (tuple) {
            if (typeof(tuple) === "string") {
                /* try to parse it */
                try {
                    tuple = JSON.parse(tuple);
                } catch (error) {
                    return;                    
                }                
            }            
            props.onChanged(tuple);
        }

        setMenu(false);
        setMenuTarget();
    };

    return (
        <div style={{ width: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", justifyContent: "space-between" }}>
                <div style={{ display: "flex", alignItems: "center" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            type="icon"
                            tooltip="Menu Options"
                            label="Open Menu"
                            icon="menu"
                            color="primary"
                            variant="text"   
                            size="small"         
                            dropdown={true}
                            onClick={toggleMenu}
                        />
                    </div>                                  
                    <div style={{ marginRight: theme.spacing(1) }} ref={(ref) => setAnnotationRef(ref)}>
                        <Button  
                            type="icon"
                            label="Show Annotation"
                            tooltip="Show Annotation"
                            icon="comment"   
                            color="primary"
                            variant="text" 
                            size="small"             
                            onClick={showAnnotation}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }} ref={(ref) => setInfoRef(ref)}>
                        <Button 
                            type="icon"
                            label="Show Help"
                            tooltip="Show Help"
                            icon="help"    
                            color="primary"
                            variant="text"   
                            size="small"         
                            onClick={showHelp}
                        />
                    </div>
                    <div>
                        <Typography 
                            style={{ fontFamily: "roboto mono" }}
                            color="primary"
                            size="3" 
                            text={props.groupHeaderText} 
                        />
                    </div>
                </div>
                <div>
                    <Button 
                        type="icon"
                        tooltip={props.expanded ? "Fold Back into Window" : "Open in New Window"}
                        label={props.expanded ? "Collapse" : "Expand"}
                        icon={props.expanded ? "call_received" : "call_made"}
                        color="primary"
                        variant="text"   
                        size="small"         
                        onClick={toggleExpand}
                    />
                </div>  
            </div>            
            <Callout   
                open={showInfoCallout}                        
                target={infoRef}
                onDismiss={(event) => { 
                    setShowInfoCallout(false); 
                    event.stopPropagation(); 
                }}
                contains={
                    <div style={{ padding: theme.spacing(2) }}>
                        <Responsive 
                            rows={[
                                {
                                    columns: [
                                        {
                                            breakpoints: {
                                                small: 12,
                                                large: 9,
                                            },
                                            contains:
                                                <Typography 
                                                    text={props.groupHeaderDescription} 
                                                />
                                        }
                                    ]
                                }
                            ]}
                        />
                    </div>
                }
            />
            <EditableCallout 
                placeholder="Click to add comments for this expression..."
                target={annotationRef}
                open={showAnnotationCallout}
                onDismiss={(event) => {setShowAnnotationCallout(false); event.stopPropagation(); }}
                value={props.tuple[0] ? props.tuple[0].description : ""}
                editable={editable}
                onChanged={props.onAnnotationChanged}
            />
            <Menu 
                target={menuTarget}
                open={menu}
                items={[
                    {
                        key: "copy",
                        label: "Copy Expression",
                        onClick: onCopy
                    },
                    {
                        key: "paste",
                        label: "Paste Expression",
                        disabled: (clipboard === undefined),
                        onClick: onPaste
                    },
                ]}
                onClose={toggleMenu}
            />
        </div>
    );
};


/*
 * This component is a design pattern that emerged to allow the UI
 *   to represent a "Group" to be presented in a GroupedList, with 
 *   collapsible parameters and support for multi-valued parameters.
 *   It handles much of the onChanged logic as well.
 *
 * Props:
 *   tuple
 *   parameters
 *
 * Where each parameter in "parameters" is an object that describes it:
 *   isMulti
 *   inline
 *   onRender
 *
 */
export const ExpressionTupleGroup = (props) => {

    const targetRef = useRef();

    const [expanded, setExpanded] = useState(props.expanded);
    const [isCollapsed, setIsCollapsed] = useState(false);
    const [/*offsetWidth*/, setOffsetWidth] = useState();
    const [showConfirmRemove, setShowConfirmRemove] = useState(false);

    const theme = useTheme();
    const groupContext = useContext(ExpressionGroupContext);
    const depth = groupContext ? groupContext.depth : 0;
    const {editable} = useOperational(props);

    /* The following can be used to determine whether to deeply 
        render an expression without enough space */
    useLayoutEffect(() => {
        if (targetRef.current) {
            setOffsetWidth(targetRef.current.offsetWidth);
        }
    }, []);

    const onClearTuple = () => {
        props.onChanged();
    };

    const onAnnotationChanged = (newValue) => {
        let tuple = props.tuple;

        if (tuple[0] && typeof(tuple[0]) === "object")
            tuple[0]["description"] = newValue;
        else
            tuple[0] = { description: newValue };
        
        props.onChanged(tuple);
    };
    
    const {tuple, contains} = props;
    if (!tuple)
        return null;

    const component = (
        <ExpressionGroupContext.Provider value={{ depth: depth + 1 }}>
            <Collapsible
                style={{ backgroundColor: (depth % 2 !== 0) ? theme.palette.background.default : undefined }}
                isCollapsed={isCollapsed}
                contains={contains}
                headerItems={
                    <ExpressionTupleGroupHeader 
                        {...props} 
                        expanded={expanded}
                        onToggleExpand={expanded => setExpanded(expanded)}
                        onAnnotationChanged={onAnnotationChanged}
                    />
                }
                onToggleCollapse={isCollapsed => setIsCollapsed(isCollapsed)}
            /> 
        </ExpressionGroupContext.Provider>
    );

    if (expanded)
        return (
            <Dialog 
                open={expanded}
                blocking={true}
                maxWidth="xl"
                contains={
                    <div style={{ height: "75vh", padding: theme.spacing(2) }}>
                        {component} 
                    </div>   
                }
            />
        );
    else {
        if (editable)
            return (
                <div style={{ minWidth: "600px" }} ref={targetRef}>
                    <div style={{ float: "left", verticalAlign: "top" }}>                    
                        <Button
                            type="icon"
                            tooltip="Remove Expression"
                            label="Remove Expression"
                            icon="close"      
                            style={{ color: theme.palette.error.main }}
                            onClick={() => setShowConfirmRemove(true)}
                            disabled={editable === false}
                        />
                    </div>
                    <div style={{ marginLeft: "46px", paddingLeft: theme.spacing(1) }}>
                        {component} 
                    </div>
                    <Dialog 
                        title="Remove Expression"
                        showClose={true}
                        open={showConfirmRemove}
                        onDismiss={() => setShowConfirmRemove(false)}
                        maxWidth="sm"
                        contains={
                            <Typography text="Are you sure you want to remove this expression?" />
                        }
                        footer={
                            <div style={{ display: "flex", paddingTop: "16px", textAlign: "right" }}>
                                <Button 
                                    style={{ marginRight: "4px" }}
                                    color="primary"
                                    label="Yes"
                                    onClick={() => {
                                        onClearTuple();
                                        setShowConfirmRemove(false);
                                    }}
                                />
                                <Button 
                                    label="No"
                                    onClose={() => setShowConfirmRemove(false)}
                                />
                            </div>
                        }
                    />
                </div>  
            );
        else 
            return (
                <div style={{ minWidth: "600px" }} ref={targetRef}>
                    { component }
                </div>
            );
        
    }
};

ExpressionTupleGroup.propTypes = {
    allowWrite:         PropTypes.bool,
    parameters:         PropTypes.arrayOf(PropTypes.any),
    tuple:              PropTypes.array,
};

export default ExpressionTupleGroup;
