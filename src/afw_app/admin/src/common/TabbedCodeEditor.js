// See the 'COPYING' file in the project root for licensing information.
import {forwardRef, createRef, useCallback, useState, useRef, useEffect} from "react";

import {
    Button,
    CodeEditor as InternalCodeEditor,
    Typography,
} from "@afw/react";

import {useApplication, useTheme, useClasses} from "../hooks";
import {useMonaco} from "@afw/react-monaco";
import {debounce} from "@afw/client";

const styles = theme => ({
    CodeEditor: {
        display: "flex", 
        flexDirection: "column", 
        height: "100%", 
        border: "1px solid " + theme.palette.background.paper,
    },
    Tab: {
        borderTopLeftRadius: "4px", 
        borderTopRightRadius: "4px", 
        cursor: "pointer",
        padding: "8px",
        boxShadow: "0px -1px 2px -3px rgba(0,0,0,0.75)",
        borderRight: "solid thin rgba(0,0,0,0.75)",
        borderLeft: "solid thin rgba(0,0,0,0.75)",
        borderTop: "solid thin rgba(0,0,0,0.5)",        
        "&:hover": {
            opacity: "0.5"
        },        
    },
    SelectedTab: {
        borderTopLeftRadius: "4px", 
        borderTopRightRadius: "4px", 
        cursor: "pointer",
        padding: "8px",
        backgroundColor: theme.palette.background.default,
        boxShadow: "0px -1px 2px 0px rgba(0,0,0,0.75)",        
    },
    NewSource: {
        padding: "8px",
        backgroundColor: theme.palette.background.paper,
        boxShadow: "0px -1px 2px -3px rgba(0,0,0,0.75)",        
    },
    ActiveTabClose: {    
    },
    TabClose: {        
        opacity: 0,
        "&:hover": {
            opacity: 1
        }
    },
    StatusBar: {
        display: "flex", 
        justifyContent: "space-between", 
        backgroundColor: theme.palette.background.paper, 
        paddingLeft: "7px", 
        paddingRight: "7px",
        borderBottomLeftRadius: "4px",
        borderBottomRightRadius: "4px"
    }
});

const statusFormat = (format) => {
    switch (format) {
    case "script":
        return "Adaptive Script";
    case "expression":
        return "Adaptive Expression";
    case "template":
        return "Adaptive Template";
    case "tuple":
        return "Adaptive Tuple";
    case "json":
        return "JSON";
    case "xml":
        return "XML";
    case "yaml":
        return "YAML";
    case "plaintext":
        return "Plain Text";
    default:
        return "Adaptive Script";
    }
};

const actionMenuOptions = [    
    {
        id: "json_stringify",
        label: "JSON Stringify",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.1
    },
    {
        id: "json_parse",
        label: "JSON Parse",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.2
    },
    {
        id: "base64_encode",
        label: "Base64 Encode",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.3
    },
    {
        id: "base64_decode",
        label: "Base64 Decode",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.4
    },
    {
        id: "uri_encode",
        label: "URI Encode",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.5
    },
    {
        id: "uri_decode",
        label: "URI Decode",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.6
    },
    {
        id: "prettify",
        label: "Prettify",
        contextMenuGroupId: "convert",
        contextMenuOrder: 1.7
    },    
];

/**
 * CodeEditor
 * 
 * Within a TabbedCodeEditor is a simple CodeEditor that works
 * with an individual piece of source.  This decomposition allows 
 * outside components to get the features of the TabbedCodeEditor
 * without necessary requiring multiple tabs.
 */
export const CodeEditor = forwardRef((props, ref) => {

    const [position, setPosition] = useState();   
    const [selection, setSelection] = useState();
    const {notification} = useApplication();    
    const {monaco} = useMonaco();
    const classes = useClasses(styles);

    const {autoFocus, editable, tab, onSourceChanged, onSaveSource, additionalActionMenuOptions} = props;
    
    /* anytime our state changes, we need to update the underlying editor callbacks */
    const editorRef = useRef();
    const [actionRefs, ] = useState({});
    const onSaveSourceRef = useRef();    
    
    const onDidChangeCursorPosition = useCallback((event) => {
        setPosition(event.position);
    }, []);

    const onDidChangeCursorSelection = useCallback(() => {
        const selection = editorRef.current.getSelection();
        const {startLineNumber, endLineNumber, startColumn, endColumn} = selection;
        if (startLineNumber !== endLineNumber || startColumn !== endColumn)
            setSelection(selection);
        else 
            setSelection();
        //setSelection(editorRef.current.getModel().getValueInRange(editorRef.current.getSelection()));
    }, []);

    /* single callback function that all addAction menu options can run */
    const onActionMenu = useCallback((actionId) => {        
        
        switch (actionId) {        
        case "json_stringify":
            onSourceChanged(JSON.stringify(tab.source));            
            break;
        case "json_parse":
            try {
                const parsed = JSON.parse(tab.source);
                if (typeof(parsed) === "string")
                    onSourceChanged(parsed);
                else
                    notification({ message: "Parsed content does not produce a string value." });
            } catch (e) {
                notification({ message: e, type: "error" });
            }
            break;
        case "base64_encode":
            onSourceChanged(window.btoa(tab.source));
            break;
        case "base64_decode":
            try {
                onSourceChanged(window.atob(tab.source));
            } catch (e) {
                notification({ message: e, type: "error" });
            }
            break;
        case "uri_encode":
            onSourceChanged(encodeURI(tab.source));
            break;
        case "uri_decode":
            onSourceChanged(decodeURI(tab.source));
            break;
        case "prettify":
            try {
                const parsed = JSON.parse(tab.source);
                onSourceChanged( JSON.stringify(parsed, null, 4));
            } catch (e) {
                notification({ message: e, type: "error" });
            }
            break;
        }
    }, [tab, onSourceChanged, notification]);

    useEffect(() => {
        if (additionalActionMenuOptions) {
            if (editorRef.current) {
                for (const {id, label, contextMenuGroupId, contextMenuOrder, run} of additionalActionMenuOptions) {
                    if (actionRefs[id] && actionRefs[id].current)
                        actionRefs[id].current.dispose();
                    else
                        actionRefs[id] = createRef();

                    actionRefs[id].current = editorRef.current.addAction({
                        id, label, contextMenuGroupId, contextMenuOrder,
                        run
                    });    
                }                            
            }
        }
    }, [additionalActionMenuOptions, actionRefs]);

    useEffect(() => {
        if (editorRef.current) {
            for (const {id, label, contextMenuGroupId, contextMenuOrder} of actionMenuOptions) {
                if (actionRefs[id] && actionRefs[id].current)
                    actionRefs[id].current.dispose();
                else
                    actionRefs[id] = createRef();

                actionRefs[id].current = editorRef.current.addAction({
                    id, label, contextMenuGroupId, contextMenuOrder,
                    run: () => onActionMenu(id)
                });                    
            }
        }        
    }, [monaco, onActionMenu, actionRefs]);

    const onCreateEditor = (editor) => {
        editorRef.current = editor;
        if (ref) ref.current = editor;

        editor.onDidChangeCursorPosition(debounce(onDidChangeCursorPosition, 200));
        editor.onDidChangeCursorSelection(onDidChangeCursorSelection);
                
        for (const {id, label, contextMenuGroupId, contextMenuOrder} of actionMenuOptions) {
            if (actionRefs[id] && actionRefs[id].current)
                actionRefs[id].current.dispose();
            else
                actionRefs[id] = createRef();                

            actionRefs[id].current = editorRef.current.addAction({
                id, label, contextMenuGroupId, contextMenuOrder,
                precondition: null, keybindingContext: null,
                run: () => onActionMenu(id)
            });                
        }

        if (additionalActionMenuOptions) {
            for (const {id, label, contextMenuGroupId, contextMenuOrder, run} of additionalActionMenuOptions) {
                if (actionRefs[id] && actionRefs[id].current)
                    actionRefs[id].current.dispose();
                else
                    actionRefs[id] = createRef();                

                actionRefs[id].current = editorRef.current.addAction({
                    id, label, contextMenuGroupId, contextMenuOrder,
                    precondition: null, keybindingContext: null,
                    run
                });                
            }
        }
        
        if (onSaveSource)
            onSaveSourceRef.current = editor.addCommand(monaco.KeyMod.CtrlCmd | monaco.KeyCode.KEY_S, onSaveSource);
    };

    return (
        <div className={classes.CodeEditor}>  
            <div style={{ flex: 1, overflow: "auto" }}>
                <InternalCodeEditor 
                    id={tab.id}           
                    data-testid={tab["data-testid"]}
                    style={{ height: "100%" }}
                    readOnly={(editable === false) ? true : false}
                    //eslint-disable-next-line
                    autoFocus={autoFocus}
                    onCreateEditor={onCreateEditor}
                    showLineNumbers={true}
                    glyphMargin={true}
                    source={tab.source}
                    onChanged={onSourceChanged}
                    language={(tab.format === "hybrid" || tab.format === "json") ? "json": (tab.format === "plaintext") ? "plaintext" : (tab.format === "xml") ? "xml" : "afw"}
                    wordWrap={true}
                    errorOffset={tab.errorOffset}
                    errorHoverMessage={tab.errorHoverMessage}
                />
            </div>
            <div className={classes.StatusBar}>                
                <Typography 
                    style={{ display: "inline-block", color: "white" }}
                    size="1" 
                    text={tab.source.length + " bytes"}
                />
                <div>                    
                    { 
                        position && 
                            <Typography 
                                style={{ display: "inline-block", marginRight: "40px", color: "white" }}
                                size="1" 
                                text={
                                    "Ln " + position.lineNumber + ", Col " + position.column + 
                                    (selection ? (" (" + editorRef.current.getModel().getValueLengthInRange(selection) + " selected)") : "")
                                } 
                            />
                    }
                    <Typography 
                        style={{ display: "inline-block", color: "white" }}
                        size="1" 
                        text={statusFormat(tab.format)} 
                    />
                </div>
            </div>
        </div>
    );
});


/**
 * TabbedCodeEditor
 *
 * This component wraps the CodeEditor to allow for multiple source inputs to be open
 * at the same time and allow them to be switched between via tabs, with the ability for 
 * a new one to be opened by clicking the "+" icon.
 * 
 * Each tab should look like the following:
 * 
 *  {
 *      label:          // the tab's label to be displayed
 *      source:         // The raw text contents of the source
 *      format:         // The source format" (afw, json, etc)
 *      errorOffset:    // The offset of any error to indicate
 *      dirty: true     // represents the contents have been saved or changed
 *  }
 */
export const TabbedCodeEditor = forwardRef((props, ref) => {

    const {        
        autoFocus,
        editable,
        tabs,
        onNewTab,
        onTabSwitch,    
        onTabClose,    
        onSourceChanged, 
        onSaveSource,        
        additionalActionMenuOptions,
    } = props;

    const theme = useTheme();
    const classes = useClasses(styles);
    const [activeTab, setActiveTab] = useState(props.activeTab ? props.activeTab : 0);    

    useEffect(() => {
        
        const onKeyUp = (e) => {            

            const filtered = [ "INPUT", "SELECT", "TEXTAREA" ];            
            const nodeName = e.srcElement.nodeName;
            const key = e.key;

            if (filtered.includes(nodeName))
                return;

            if (nodeName && (key === "n" || key === "N")) {
                onNewTab();
            }
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, [onNewTab]);

    useEffect(() => {
        setActiveTab(props.activeTab);
    }, [props.activeTab]);

    return (
        <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", backgroundColor: theme.palette.background.paper }}>
                {
                    tabs.map( (tab, index) => (
                        <div 
                            role="tab"
                            tabIndex={0}
                            className={(index === activeTab) ? classes.SelectedTab : classes.Tab}
                            key={index}
                            onKeyDown={() => {
                                if (onTabSwitch)
                                    onTabSwitch(index);                                
                                setActiveTab(index);
                            }}
                            onClick={() => {
                                if (onTabSwitch)
                                    onTabSwitch(index);
                                setActiveTab(index);
                            }}                            
                        >
                            <div style={{ display: "flex", alignItems: "center" }}>
                                <div style={{ paddingLeft: "4px" }}>
                                    <Typography 
                                        style={{ fontWeight: (index === activeTab) ? "900" : undefined }} 
                                        color={(index !== activeTab) ? "textSecondary" : undefined}
                                        text={(tab.label ? tab.label : "Untitled-" + (index + 1)) + (tab.dirty ? " *" : "")}                                 
                                        size="3" 
                                    />
                                </div>                                
                                {
                                    (tab.canClose !== false) &&
                                        <Button 
                                            className={(index === activeTab) ? classes.ActiveTabClose : classes.TabClose}                                 
                                            size="small" 
                                            type="icon" 
                                            icon="close" 
                                            tooltip="Close Source Window"
                                            label="Close Source Window"
                                            onClick={(event) => {   
                                                onTabClose(index);                                 
                                                event.stopPropagation();
                                            }}
                                        />    
                                }      
                            </div>           
                        </div>                        
                    ))
                }
                <div className={classes.NewSource}>
                    { onNewTab &&
                        <Button                             
                            size="small" 
                            type="icon" 
                            icon="add" 
                            color="primary"
                            tooltip="New Source Window"
                            label="New Source Window"
                            onClick={() => {
                                onNewTab();                                                        
                            }}                        
                        />
                    }
                </div>
            </div>
            {
                (activeTab !== undefined) && tabs[activeTab] &&
                    <div style={{ flex: 1, overflow: "auto" }}>
                        <CodeEditor                         
                            ref={ref}    
                            // eslint-disable-next-line jsx-a11y/no-autofocus
                            autoFocus={autoFocus}
                            tab={tabs[activeTab]}
                            editable={editable}
                            onSaveSource={onSaveSource}
                            onSourceChanged={onSourceChanged}
                            additionalActionMenuOptions={additionalActionMenuOptions}
                        />
                    </div>
            }            
        </div>
    );
});

export default TabbedCodeEditor;
