// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Menu,
} from "@afw/react";

import {useApplication, useTheme} from "../../hooks";

import ContextEditor from "./ContextEditor";
import FlagsSelector from "../../common/FlagsSelector";

/**
 * FiddleToolbar
 * 
 * This component renders the entire Toolbar used by Fiddle, including all Menu options and actions.
 */
export const FiddleToolbar = (props) => {
   
    const {
        onNewFile,
        storage,
        onOpenRecent,
        onSaveFile,
        onSaveFileAs,
        canDelete,
        onDeleteScript,
        onOpen,
        selectedEditor,   
        inputLabel,         
        input,
        inputFormat,
        onChangeInputFormat,
        onList,
        canList,
        onEvaluate,
        canEvaluate,
        selectedFlags,
        setSelectedFlags,
        contextTypes,
        contextVariables,        
        onSelectEditor,
        setContextVariables,
        onTabClose,
        onTabCloseAll,
        setSelectedContextTypes,
    } = props;

    const [optionsTarget, setOptionsTarget] = useState();
    const [showContext, setShowContext] = useState(false);
    const {isMobile} = useApplication();
    const theme = useTheme();


    const onMenuOption = callback => () => {
        setOptionsTarget();
        callback();        
    };

    return (
        <div style={{ display: "flex", alignItems: "center" }}>
            <div>
                <Button 
                    style={{ marginRight: theme.spacing(0.5) }}
                    type="icon"
                    icon="more_vert"
                    tooltip="More Options"
                    label="More Options"
                    onClick={(event) => setOptionsTarget(event.target)} 
                />
                <Menu 
                    open={Boolean(optionsTarget)}
                    onClose={() => setOptionsTarget()}
                    target={optionsTarget}                    
                    items={[
                        {
                            key: "New",
                            label: "New",
                            subMenu: {
                                items: [
                                    {
                                        key: "script",
                                        label: "Adaptive Script",
                                        onClick: onMenuOption(() => onNewFile("script")),
                                    },
                                    {
                                        key: "test-script",
                                        label: "Adaptive Test Script",
                                        onClick: onMenuOption(() => onNewFile("script", "test")),
                                    },
                                    {
                                        key: "expression",
                                        label: "Adaptive Expression",
                                        onClick: onMenuOption(() => onNewFile("expression")),
                                    },
                                    {
                                        key: "template",
                                        label: "Adaptive Template",
                                        onClick: onMenuOption(() => onNewFile("template"))
                                    },
                                    {
                                        key: "json",
                                        label: "JSON",
                                        onClick: onMenuOption(() => onNewFile("json"))
                                    }
                                ]
                            }
                        },                        
                        {
                            key: "Open",
                            label: "Open",
                            onClick: onMenuOption(onOpen)
                        },
                        {
                            key: "OpenRecent",
                            label: "Open Recent",
                            subMenu: {
                                items: storage && storage.recent ? storage.recent.map(({ label, path, isLocal }) => ({
                                    key: path,
                                    label,
                                    description: path,
                                    onClick: onMenuOption(() => onOpenRecent({ label, path, isLocal }))
                                })) : [],
                            },
                            
                            disabled: (!storage || !storage.recent || storage.recent.length === 0)
                        },
                        {
                            key: "Save",
                            label: "Save",
                            disabled: (!input || input.length === 0),
                            onClick: onMenuOption(onSaveFile)
                        },
                        {
                            key: "SaveAs",
                            label: "Save As...",
                            disabled: (!input || input.length === 0),
                            onClick: onMenuOption(onSaveFileAs)
                        },
                        {
                            key: "Delete",
                            label: "Delete" + (inputLabel ? (" \"" + inputLabel + "\"") : ""),
                            disabled: !canDelete,
                            onClick: onMenuOption(onDeleteScript)
                        },                        
                        {
                            key: "divider1",
                            type: "divider"  
                        },
                        {
                            key: "Format",
                            label: "Format",
                            disabled: !inputFormat,
                            subMenu: {
                                items: [
                                    {
                                        key: "script",
                                        label: "Adaptive Script",      
                                        canCheck: true,            
                                        checked: (inputFormat === "script"),                 
                                        onClick: () => {
                                            onChangeInputFormat("script");
                                            setOptionsTarget();
                                        }
                                    },
                                    {
                                        key: "expression",
                                        label: "Adaptive Expression",     
                                        canCheck: true,             
                                        checked: (inputFormat === "expression"),    
                                        onClick: () => {
                                            onChangeInputFormat("expression");
                                            setOptionsTarget();
                                        }
                                    },
                                    {
                                        key: "template",
                                        label: "Adaptive Template",    
                                        canCheck: true,   
                                        checked: (inputFormat === "template"),
                                        onClick: () => {
                                            onChangeInputFormat("template");
                                            setOptionsTarget();
                                        }
                                    },
                                    {
                                        key: "json",
                                        label: "JSON",        
                                        canCheck: true,        
                                        checked: (inputFormat === "json"),
                                        onClick: () => {
                                            onChangeInputFormat("json");
                                            setOptionsTarget();
                                        }
                                    },
                                ]
                            }
                        },
                        {
                            key: "Editor",
                            label: "Editor",
                            subMenu: {
                                items: [
                                    {
                                        key: "source",
                                        label: "Source",
                                        canCheck: true,
                                        checked: (selectedEditor === "source"),
                                        onClick: onMenuOption(() => onSelectEditor("source"))
                                    },
                                    {
                                        key: "builder",
                                        label: "Builder",
                                        canCheck: true,
                                        checked: (selectedEditor === "builder"),
                                        onClick: onMenuOption(() => onSelectEditor("builder"))
                                    }
                                ]
                            }
                        },
                        {
                            key: "divider2",
                            type: "divider",
                        },
                        {
                            key: "close",
                            label: "Close Tab",
                            onClick: onMenuOption(onTabClose),
                        },
                        {
                            key: "close_all",
                            label: "Close All Tabs",
                            onClick: onMenuOption(onTabCloseAll),
                        },
                        {
                            key: "divider3",
                            type: "divider"
                        },
                        {
                            key: "copy",
                            label: "Copy Input",  
                            disabled: true,                             
                        },
                        {
                            key: "paste",
                            label: "Paste",   
                            disabled: true,                            
                        },
                    ]}
                />
            </div>            
            <div style={{ marginRight: theme.spacing(0.5) }}>
                <Button          
                    type={isMobile ? "icon": undefined}
                    tooltip="Evaluate the Expression"    
                    variant="text"            
                    color="primary"
                    label="Evaluate"
                    icon="play_arrow"
                    disabled={!canEvaluate}                    
                    onClick={onMenuOption(onEvaluate)}
                />  
            </div>
            <div style={{ marginRight: theme.spacing(0.5) }}>
                <Button       
                    type={isMobile ? "icon": undefined}
                    variant="text"  
                    color="primary"
                    tooltip="Get Compiler Listing"    
                    label="Listing"
                    icon="format_list_bulleted"
                    disabled={!canList}                    
                    onClick={onMenuOption(onList)}
                />  
            </div>
            <div style={{ marginRight: theme.spacing(0.5) }}>
                <Button   
                    type={isMobile ? "icon": undefined}
                    variant="text"                                      
                    tooltip="Configure evaluation context."                        
                    label="Context" 
                    icon="code"                                                                                                 
                    onClick={onMenuOption(() => setShowContext(true))}                                                                                                          
                /> 
            </div>
            <div style={{ marginRight: theme.spacing(0.5) }}>
                <FlagsSelector 
                    selectedFlags={selectedFlags}
                    onSelectFlags={setSelectedFlags}
                />
            </div>
            <ContextEditor 
                open={showContext}
                onDismiss={() => setShowContext(false)}
                contextVariables={contextVariables}
                contextTypes={contextTypes}
                onSave={(contextVariables, selectedContextTypes) => { 
                    setContextVariables(contextVariables);
                    setSelectedContextTypes(selectedContextTypes);
                    setShowContext(false);
                }}
            /> 
        </div>
    );
};

export default FiddleToolbar;
