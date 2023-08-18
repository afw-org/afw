// See the 'COPYING' file in the project root for licensing information.
import {    
    useState, 
    useEffect, 
    useReducer, 
    useRef, 
    useCallback,     
} from "react";
import {Prompt} from "react-router";

import {
    Button,
    Dialog,
    HybridBuilder,
    QualifiersProvider,
    Spinner,
    Typography,   
    useAdaptors, 
    useFunctions,
    useContextTypes,
    useClipboard,
    OperationalContext,
    OperationalMode
} from "@afw/react";

import {
    afwEvaluateExpression, 
    afwEvaluateTemplate,
    afwCompileExpression,
    afwCompileScript,
    afwCompileTemplate,
    afwGetObjectWithUri,
    afwReplaceObjectWithUri,
    afwAddObjectWithUri,
} from "@afw/core";

import {AfwClient, debounce} from "@afw/client";
import {useApplication, useLocalJSONStorage, useTheme} from "../../hooks";

import {FiddleToolbar} from "./FiddleToolbar";
import {EvaluationOutput} from "./FiddleOutput";
import {OpenFileDialog} from "./OpenFileDialog";
import {SaveFileDialog} from "./SaveFileDialog";
import TabbedCodeEditor from "../../common/TabbedCodeEditor";
import SplitPane from "../../common/SplitPane";

const FIDDLE_MAX_RECENT = 8;

/* default object to save to Local Storage in browser */
const fiddleLocalStorageDefault = { scripts: {}, recent: [] };

/* default initial fiddle state */
const initialState = {
    initialHeight:          0,
    defaultPanelSize:       "100%",
    tabs:                   [],
    selectedFlags:          [ 
        "response:error", 
        "response:stdout", 
        "response:stderr",
        "response:stdout:stream",
        "response:stderr:stream",
        "response:console:stream"
    ],
    selectedEditor:         "source",
    dirty:                  false,
    counter:                1,
    selectedContextTypes:   [],
    contextVariables:       [],
    hasOutput:              false,
};

/* grab the initial source for each type of new file */
const getInitialSource = (format, subformat) => {

    if (format === "script" && subformat === "test") {
        return (
            `#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: script_name.as
//? customPurpose: Part of custom tests
//? description: This script is used to test some functionality
//? sourceType: script
//?
//? test: test_1
//? description: This is the first test
//? expect: 0
//? source: ...
#!/usr/bin/env afw


return 0;`
        );
    }

    /* default to empty string */
    return "";
};

/**
 * Determine if the given source is a test script content
 */
const isTestScript = (source) => {

    const newline = source.indexOf("\n");
    if (newline > 0) {
        if (source.substring(0, newline).indexOf("afw --syntax test_script") > 0)
            return true;
    }

    return false;
};

const getError = (error) => {

    if (error) {
        if (typeof error === "string")
            return { message: error };
        else if (error instanceof Error) 
            return { message: error.message };
        else
            return { ...error };
    }
};

/**
 * The main Reducer function for Fiddle.  It takes a state and action and
 * returns the new state for the Fiddle app.
 */
const reducer = (state, action) => {
    
    switch (action.type) {

    case "INIT_HEIGHT":
        return ({
            ...state,
            initialHeight: action.height
        });

    case "LOADING_FUNCTIONS_VARIABLES":
        return ({
            ...state,
            spinnerText: "Loading Functions and Variables",
        });

    case "LOADED_FUNCTIONS_VARIABLES":
        return ({
            ...state,
            spinnerText: undefined
        });

    case "OPEN_OUTPUT": {    
        const defaultPanelSize = (state.defaultPanelSize === "100%") ? 
            0.6 * state.initialHeight : state.defaultPanelSize;

        return ({
            ...state,
            defaultPanelSize,
            outputPanelSize: state.initialHeight - defaultPanelSize,
            stdout: action.stdout,
            stderr: action.stderr,
            listing: action.listing,
            error: getError(action.error),
            spinnerText: undefined,   
            hasOutput: true,                  
        });

    }

    case "CLOSE_OUTPUT": {
        const updatedTabs = [ ...state.tabs ];
        updatedTabs.forEach( t => {
            if (t.errorOffset !== undefined)
                t.errorOffset = undefined;
            if (t.errorHoverMessage !== undefined)
                t.errorHoverMessage = undefined;
        });

        return ({
            ...state,
            defaultPanelSize: "100%",
            outputPanelSize: 0,
            error: undefined,
            result: undefined,
            resultDataType: undefined,
            stdout: undefined,
            stderr: undefined,
            listing: undefined,            
            tabs: updatedTabs,
            hasOutput: false,
        });

    } 
    
    case "EVALUATING": {
        const updatedTabs = [ ...state.tabs ];
        const currentTab = updatedTabs[state.activeTab];
        currentTab.errorOffset = undefined;
        currentTab.errorHoverMessage = undefined;

        return ({
            ...state,
            spinnerText: action.spinnerText,
            error: undefined,
            listing: undefined,
        });

    } 
    
    case "EVALUATED": {
        const updatedTabs = [ ...state.tabs ];
        const currentTab = updatedTabs[state.activeTab];        

        let evaluatedSubformat = action.subformat;
        if (!evaluatedSubformat) {
            if (isTestScript(currentTab.source))
                evaluatedSubformat = "test";            
        }

        return ({
            ...state,
            spinnerText: undefined,
            result: action.result,
            resultDataType: action.result,
            tabs: updatedTabs,
            evaluatedFormat: action.format,
            evaluatedSubformat,
            hasOutput: true,
        });

    } 
    case "LISTING":
        return ({
            ...state,
            spinnerText: action.spinnerText,
            error: undefined,
            result: undefined,
            resultDataType: undefined,
        });


    /* \fixme "ERROR" may indicate an internal Fiddle error (caught exception) - handle separately */
    case "ERROR": 
        return ({
            ...state,
            spinnerText: undefined,
            error: (typeof action.error === "string") ? 
                { message: action.error } : 
                { message: action.error.message },
            stdout: undefined,
            stderr: undefined,
            result: undefined,
            resultDataType: undefined,
            listing: undefined,
            showSaveAs: false,
            hasOutput: true,
        });

    case "ERROR_OFFSET":
        return ({
            ...state,
            tabs: action.updatedTabs,
        });

    case "SCRIPT_SAVED":            
        return ({
            ...state,
            tabs: action.updatedTabs,
            showSaveAs: false,
        });

    case "SAVE_AS":
        return ({
            ...state,
            showSaveAs: true,
        });

    case "DISMISS_SAVE_AS":
        return ({
            ...state,
            showSaveAs: false,
        });

    case "NEW_FILE":
        return ({
            ...state,            
            tabs: [ ...state.tabs, { 
                source: getInitialSource(action.format, action.subformat), 
                format: action.format ? action.format : "script", 
                subformat: action.subformat,
                label: "Untitled-" + state.counter,
                inStorage: false,
            }],
            activeTab: state.tabs.length,         
            counter: state.counter + 1,            
        });

    case "OPEN_SCRIPT":
        return ({
            ...state,
            tabs: [
                ...state.tabs,
                action.newTab
            ],
            activeTab: state.tabs.length,     
            showOpen: false,       
        });    

    case "SHOW_OPEN":
        return ({
            ...state,
            showOpen: true,
        });

    case "DISMISS_FILE_OPEN":
        return ({
            ...state,
            showOpen: false,
        });

    case "SAVING_FILE": 
        return ({
            ...state,
            spinnerText: "Saving File..."
        });

    case "SAVED_FILE": {
        return ({
            ...state,
            spinnerText: undefined,
            tabs: action.updatedTabs,
            showSaveAs: false,
        });
    }

    case "LOADING_VFS_FILE":
        return ({
            ...state,
            showOpen: false,
            spinnerText: "Loading File..."
        });

    case "LOADED_VFS_FILE": {
        const tabs = [ 
            ...state.tabs,
            action.newTab
        ];        

        return ({
            ...state,
            spinnerText: undefined,
            tabs,
            activeTab: state.tabs.length,
        });

    } case "TAB_CLOSE":        
        return ({
            ...state,
            showConfirmClose: true,            
        });

    case "DISMISS_CLOSE":
        return ({
            ...state,
            showConfirmClose: false,
        });

    case "CONFIRM_CLOSE":
        return ({
            ...state,
            tabs: action.updatedTabs,
            activeTab: action.newActiveTab,
            showConfirmClose: false,
            dirty: (action.updatedTabs.length === 0) ? false : state.dirty
        });

    case "CLOSE_ALL":
        return ({
            ...state,
            showConfirmCloseAll: true,            
        });

    case "DISMISS_CLOSE_ALL":
        return ({
            ...state,
            showConfirmCloseAll: false
        });

    case "CONFIRM_CLOSE_ALL":
        return ({
            ...state,
            tabs: [],
            activeTab: undefined,
            showConfirmCloseAll: false,
            dirty: false,
        });

    case "DELETE":
        return ({
            ...state,
            showConfirmDelete: true,
        });

    case "CONFIRM_DELETE": 
        return ({
            ...state,
            showConfirmDelete: false,
        });

    case "DISMISS_DELETE":
        return ({
            ...state,
            showConfirmDelete: false,
        });

    case "SPLIT_RESIZE":
        return ({
            ...state,
            defaultPanelSize: action.defaultPanelSize,
            outputPanelSize: state.initialHeight - action.defaultPanelSize,
        });

    case "SOURCE_CHANGED": {
        let updatedTabs = [ ...state.tabs ];

        if (action.newValue !== updatedTabs[state.activeTab].source) {
            updatedTabs[state.activeTab].dirty = true;
        }

        /* clear any errors, if user begins typing */
        if (updatedTabs[state.activeTab].errorOffset !== undefined) {
            updatedTabs[state.activeTab].errorOffset = undefined;
            updatedTabs[state.activeTab].errorHoverMessage = undefined;
        }

        updatedTabs[state.activeTab].source = action.newValue;

        return ({
            ...state,
            tabs: updatedTabs,
            dirty: true
        });

    } case "TAB_SWITCH":
        return ({
            ...state,
            activeTab: action.activeTab,
        });

    case "CHANGE_FORMAT": {
        let updatedTabs = [ ...state.tabs ];
        updatedTabs[state.activeTab].format = action.format;

        return ({
            ...state,
            tabs: updatedTabs
        });

    } case "SET_SELECTED_FLAGS":
        return ({
            ...state,
            selectedFlags: action.selectedFlags,
        });

    case "SET_CONTEXT_VARIABLES":
        return ({
            ...state,
            contextVariables: action.contextVariables,
        });

    case "SET_SELECTED_CONTEXT_TYPES":
        return ({
            ...state,
            selectedContextTypes: action.selectedContextTypes,
        });

    case "SET_SELECTED_EDITOR":
        return ({
            ...state,
            selectedEditor: action.selectedEditor,
        });

    default:
        return state;
    }
};

const ConfirmDeleteDialog = ({ open, onDismiss, onConfirm, label }) => 
{
    const theme = useTheme();

    return (
        <Dialog 
            open={open}
            title="Delete Permanently"
            isBlocking={true}
            onDismiss={onDismiss}
            maxWidth="sm"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Typography text={"Are you sure you wish to delete " + label + "?"} />
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div>
                        <Button 
                            variant="contained"
                            color="primary"
                            label="Delete"
                            onClick={onConfirm}
                        />
                    </div>
                    <div style={{ paddingLeft: theme.spacing(1) }}>
                        <Button
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>                            
                </div>
            }
        />
    );
};

const ConfirmCloseDialog = ({ open, label, onDismiss, onConfirm }) => {

    const theme = useTheme();

    return (
        <Dialog 
            open={open}
            title="Close Tab"
            isBlocking={true}
            onDismiss={onDismiss}
            maxWidth="sm"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Typography text={"You have unsaved work.  Are you sure you wish to close \"" + label + "\"?"} />
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div>
                        <Button 
                            variant="contained"
                            color="primary"
                            label="Close"
                            onClick={onConfirm}
                        />
                    </div>
                    <div style={{ paddingLeft: theme.spacing(1) }}>
                        <Button
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>                            
                </div>
            }
        />
    );
};

const ConfirmCloseAllDialog = ({ open, onDismiss, onConfirm }) => {

    const theme = useTheme();

    return (
        <Dialog 
            open={open}
            title="Close All Tabs"
            isBlocking={true}
            onDismiss={onDismiss}
            maxWidth="sm"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Typography text={"Are you sure you wish to close all tabs?"} />
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div>
                        <Button 
                            variant="contained"
                            color="primary"
                            label="Close"
                            onClick={onConfirm}
                        />
                    </div>
                    <div style={{ paddingLeft: theme.spacing(1) }}>
                        <Button
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>                            
                </div>
            }
        />
    );
};

const ErrorFunctions = () =>
    <div>
        <div style={{ height: "10vh" }} />
        <div style={{ textAlign: "center" }}>
            <Typography color="error" size="7" text="Error Loading Functions" />
            <Typography color="error" text="Fiddle requires adaptive function definitions to operate." />
        </div>
    </div>;
    

/**
 * Fiddle provides a sandbox to try out Adaptive Scripts, Expressions or 
 * Templates.
 */
export const Fiddle = () => {

    const client                                        = useRef();
    const editorRef                                     = useRef();
    const splitPaneRef                                  = useRef();
     
    const [hybrid, setHybrid]                           = useState();  

    const theme                                         = useTheme();
    const {functions, error: errorFunctions}            = useFunctions();
    const {adaptors : vfsAdaptors = []}                 = useAdaptors("vfs");
    const {contextTypes}                                = useContextTypes();
    const {notification}                                = useApplication();    
    const {onCopy}                                      = useClipboard();
    const {storage, onChanged: onStorageChanged}        = useLocalJSONStorage("fiddle", fiddleLocalStorageDefault);             
    const [state, dispatch]                             = useReducer(reducer, initialState);    

    const activeTabContent = (state.tabs.length > 0) ? state.tabs[state.activeTab] : undefined;
    const activeTabLabel = activeTabContent?.label;
    const input = activeTabContent?.source;
    const activeTabInputFormat = activeTabContent?.format;
    const activeTabInputSubformat = activeTabContent?.subformat;

    useEffect(() => {
        client.current = new AfwClient({
            url: "/afw",
            accept: "application/x-afw",
        });        
    }, []);

    useEffect(() => {
        dispatch({ type: "INIT_HEIGHT", height: splitPaneRef.current.clientHeight });
    }, []);

    /**
     *  Create the session to be used by Fiddle and assign the selected flags.     
     */
    useEffect(() => {
        if (state.selectedFlags)
            client.current.setActionFlags(state.selectedFlags);
    }, [state.selectedFlags]);

    /* User spinner until functions and contextTypes are loaded */
    useEffect(() => {
        if (functions && contextTypes)
            dispatch({ type: "LOADED_FUNCTIONS_VARIABLES" });
        else 
            dispatch({ type: "LOADING_FUNCTIONS_VARIABLES" });

    }, [functions, contextTypes]);

    /**
     * This is called when the Evaluate button is clicked for a script.
     */
    const onEvaluateScript = async (script, additionalUntrustedQualifiedVariables) => {        
        let action = {};

        action["function"] = "evaluate<script>";
        action["source"] = script;

        if (additionalUntrustedQualifiedVariables)
            action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

        try {
            const streams = await client.current.perform(action).streams();                                           
            const {response: { result, resultDataType, error }, stdout, stderr} = streams;

            dispatch({ type: "OPEN_OUTPUT", stdout, stderr, error });

            if (error) {
                const updatedTabs = [ ...state.tabs ];
                if (error.offset !== undefined)
                    updatedTabs[state.activeTab].errorOffset = error.offset;
                else 
                    updatedTabs[state.activeTab].errorOffset = error.parserCursor - 1;
                updatedTabs[state.activeTab].errorHoverMessage = error.message;
                
                dispatch({ type: "ERROR_OFFSET", updatedTabs });            
            }

            return { result, resultDataType };
        } catch (e) {            
            dispatch({ type: "OPEN_OUTPUT", error: e });            
        }

        return {};
    };

    /**
     * This is called when the Evaluate button is clicked.
     */
    const onEvaluate = async () => {

        dispatch({ type: "EVALUATING", spinnerText: "Evaluating..." });

        /* use context variables to build up an evaluation context */
        let additionalUntrustedQualifiedVariables = (state.contextVariables.length > 0) ? {} : undefined;
        state.contextVariables.forEach(contextVariable => {
            const variable = contextVariable.variable;
            const value = contextVariable.value;

            if (variable.includes("::")) {
                const qualifier = variable.substring(0, variable.indexOf("::"));
                const name = variable.substring(variable.indexOf("::") + 2);

                if (!additionalUntrustedQualifiedVariables[qualifier])
                    additionalUntrustedQualifiedVariables[qualifier] = {};
                
                additionalUntrustedQualifiedVariables[qualifier][name] = value;
            } else {
                /* unqualified */
                if (!additionalUntrustedQualifiedVariables[""])
                    additionalUntrustedQualifiedVariables[""] = {};
                additionalUntrustedQualifiedVariables[""][variable] = value;
            }
        });

        try {
            let result, resultDataType;

            if (activeTabInputFormat === "script") 
                ({ result, resultDataType } = 
                    await onEvaluateScript(input, additionalUntrustedQualifiedVariables));

            else if (activeTabInputFormat === "expression") {                
                result = await afwEvaluateExpression(client.current, input, additionalUntrustedQualifiedVariables).result();                
            }

            else if (activeTabInputFormat === "template") {
                result = await afwEvaluateTemplate(client.current, input, additionalUntrustedQualifiedVariables).result();                
            }

            dispatch({ 
                type: "EVALUATED", 
                /* \fixme move to reducer and handle NaN/Infinity/etc.  */
                result: (result === undefined) ? "undefined" : JSON.stringify(result, null, 4),
                resultDataType,
                format: activeTabInputFormat,
                subformat: activeTabInputSubformat,
            });

        } catch (error) {

            dispatch({ type: "ERROR", error });

            if (error) {
                const updatedTabs = [...state.tabs];
                if (error.offset !== undefined)
                    updatedTabs[state.activeTab].errorOffset = error.offset;
                else 
                    updatedTabs[state.activeTab].errorOffset = error.parserCursor - 1;
                updatedTabs[state.activeTab].errorHoverMessage = error.message;
                
                dispatch({ type: "ERROR_OFFSET", updatedTabs });
            }            
        }
    };

    /**
     * This is called when the Listing button is clicked.
     */
    const onList = async () => {
        dispatch({ type: "LISTING", spinnerText: "Listing..." });

        try {
            let result;

            if (activeTabInputFormat === "script") {
                result = await afwCompileScript(client.current, input, 4).result();                
            }

            else if (activeTabInputFormat === "expression") {
                result = await afwCompileExpression(client.current, input, 4).result();
            }

            else if (activeTabInputFormat === "template") {
                result = await afwCompileTemplate(client.current, input, 4).result();                
            }
            
            dispatch({ type: "OPEN_OUTPUT", listing: result });

        } catch (error) {
            dispatch({ type: "ERROR", error });
        }
    };

    /**
     * Called whenever we need to add a file to the recent list for Open Recent access.
     */
    const addRecentFile = ({ label, path, isLocal }) => {
        if (!storage.recent)
            storage.recent = [];
        
        /* look to see if it's already in recent */
        let found = false;
        for (const entry of storage.recent) {
            if (entry.path === path)
                found = true;
        }

        if (!found) {
            if (storage.recent.length >= FIDDLE_MAX_RECENT)
                storage.recent.shift();
            storage.recent.push({ label, path, isLocal });
            onStorageChanged(storage);
        }
    };

    /**
     * This is called when a Script is saved.
     */
    const onSaveLocalFile = (scriptName) => {
        let newStorage = { ...storage };
        let name = scriptName ? scriptName : activeTabLabel;
        let updatedTabs = [ ...state.tabs ];
        
        const activeTabContent = state.tabs[state.activeTab];
        
        newStorage["scripts"][name] = {
            input: activeTabContent.source,
            inputFormat: activeTabContent.format,
            timestamp: (new Date()).toString()
        };
        
        updatedTabs[state.activeTab].dirty = false;
        updatedTabs[state.activeTab].label = name;
        updatedTabs[state.activeTab].inStorage = true;

        onStorageChanged(newStorage);        
        dispatch({ type: "SCRIPT_SAVED", updatedTabs, newStorage });            

        notification({ message: "File saved.", type: "success", duration: 1000 });
    };

    /**
     * onCreateVfsFile will be called when a file is Saved As a new filename.
     */
    const onCreateVfsFile = async (fileName, vfsPath) => {

        const activeTabContent = state.tabs[state.activeTab];
        const path = vfsPath ? vfsPath : activeTabContent.vfsPath;
        const name = fileName ? fileName : activeTabLabel;

        try {
            dispatch({ type: "SAVING_FILE" });

            await afwAddObjectWithUri(
                client.current, path + "%2F" + fileName, 
                { data: activeTabContent.source }
            ).result();

            let updatedTabs = [ ...state.tabs ];

            updatedTabs[state.activeTab].dirty = false;
            updatedTabs[state.activeTab].label = name;
            updatedTabs[state.activeTab].inStorage = true;

            dispatch({ type: "SAVED_FILE", updatedTabs });            

            notification({ message: "File saved on server.", type: "success", duration: 1000 });
        } catch (e) {
            dispatch({ type: "ERROR", error: e });
        }
    };

    /**
     * onSaveVfsFile will be called when saving a file that already exists.
     */
    const onSaveVfsFile = async (scriptName, vfsPath) => {        
        
        const activeTabContent = state.tabs[state.activeTab];
        const path = vfsPath ? vfsPath : activeTabContent.vfsPath;
        const name = scriptName ? scriptName : activeTabLabel;

        try {
            dispatch({ type: "SAVING_FILE" });

            await afwReplaceObjectWithUri(
                client.current,
                path, 
                { data: activeTabContent.source }
            ).result();

            let updatedTabs = [ ...state.tabs ];

            updatedTabs[state.activeTab].dirty = false;
            updatedTabs[state.activeTab].label = name;
            updatedTabs[state.activeTab].inStorage = true;

            dispatch({ type: "SAVED_FILE", updatedTabs });            

            notification({ message: "File saved on server.", type: "success", duration: 1000 });
        } catch (e) {
            dispatch({ type: "ERROR", error: e });
        }
    };

    /**
     * This is called when the Save As option is selected.
     */
    const onSaveFileAs = () => {
        dispatch({ type: "SAVE_AS" });        
    };    

    /**
     * This is called when 'Save' option is selected.  It may be Untitled- at this point,
     * so we determine if we should Save As or just Save.
     */
    const onSaveFile = () => {        
        
        if (activeTabContent?.inStorage === false && !activeTabContent?.vfsPath)
            onSaveFileAs();                 
        else if (activeTabContent?.vfsPath)
            onSaveVfsFile();
        else        
            onSaveLocalFile();            
    };

    /**
     * This is called when the 'Open' option is selected to open up an existing file.
     */
    const onOpenLocalFile = (label) => {

        const {input, inputFormat} = storage.scripts[label];

        const newTab = {
            label, 
            source: input,
            format: inputFormat ? inputFormat : "script"
        };

        addRecentFile({ label, path: "/LocalStorage/" + label, isLocal: true });

        dispatch({ type: "OPEN_SCRIPT", label, newTab });
    };

    /**
     * This is called when opening a VFS file from the browse dialog box.
     */
    const onOpenFile = async (path, isLocal) => {               

        if (isLocal)
            return onOpenLocalFile(path.split("/").pop());

        dispatch({ type: "LOADING_VFS_FILE" });

        try {
            const file = await afwGetObjectWithUri(client.current, path, { objectId: true }).result();            
            const label = file._meta_.objectId.split("/").pop();
            const newTab = {
                label, 
                source: file.data,
                format: path.endsWith(".as") ? "script" : "plaintext",
                vfsPath: path,
            };

            addRecentFile({ label, path, isLocal });
            
            dispatch({ type: "LOADED_VFS_FILE", vfsPath: path, file, newTab });        
        } catch (error) {
            dispatch({ type: "ERROR", error });
        }        
    };

    /**
     * This is called when the 'Open Recent' option is selected to open up a recently opened file.
     */    
    const onOpenRecentFile = ({ path, isLocal }) => {
        onOpenFile(path, isLocal);
    };

    /**
     * This is called when the 'Open Server' option is selected to open a VFS file.
     */
    const onOpen = () => {
        dispatch({ type: "SHOW_OPEN" });
    };

    /**
     * This is called when the 'Delete' option is selected.
     */
    const onDeleteScript = () => {
        dispatch({ type: "DELETE" });
    };

    /**
     * This is called after 'Close' is selected and confirmed.
     */
    const onTabCloseConfirm = (tabIndex = state.activeTab) => {

        let updatedTabs = [ ...state.tabs ];
        updatedTabs.splice(tabIndex, 1);
        let newActiveTab = state.activeTab;
                    
        if (tabIndex < state.activeTab) {
            newActiveTab--;        
        } else if (newActiveTab >= updatedTabs.length) 
            newActiveTab = updatedTabs.length - 1;

        if (newActiveTab < 0) {
            newActiveTab = updatedTabs.length ? 0 : undefined;
        }
                
        dispatch({ type: "CONFIRM_CLOSE", updatedTabs, newActiveTab });
    };

    /**
     * This is called when a Tab is closed.
     */
    const onTabClose = (tabIndex) => {
        if (state.tabs[tabIndex].dirty) {
            dispatch({ type: "TAB_CLOSE", tabIndex });
        } else {
            onTabCloseConfirm(tabIndex);
        }
    };

    /**
     * This is called when an open Tab is deleted and confirmed.
     */
    const onConfirmDelete = () => {
        let newStorage = { ...storage };
        delete storage["scripts"][activeTabLabel];
        
        onTabClose(state.activeTab);
             
        onStorageChanged(newStorage);

        dispatch({ type: "CONFIRM_DELETE" });
    };

    /**
     * This is called when a New source is opened.
     */
    const onNewFile = (format, subformat) => {
        dispatch({ type: "NEW_FILE", format, subformat });        

        if (editorRef.current)
            editorRef.current.focus();
    };

    /**
     * This is called after 'Close All' is selected and confirmed.
     */
    const onTabCloseAllConfirm = () => {
        dispatch({ type: "CONFIRM_CLOSE_ALL" });
    };

    /**
     * This is called after 'Close All' is selected.
     */
    const onTabCloseAll = () => {
        dispatch({ type: "CLOSE_ALL" });
    };

    const onCopyAction = useCallback(() => {        
        let action = {};

        if (state.selectedFlags && state.selectedFlags.length > 0)
            action._flags_ = state.selectedFlags;

        if (state.contextVariables && state.contextVariables.length > 0)
            action.additionalUntrustedQualifiedVariables = state.contextVariables;

        action.function = "evaluate<script>";
        action.source = state.tabs[state.activeTab].source;
        
        onCopy(JSON.stringify(action, null, 4));
    }, [onCopy, state]);

    /* if we were unable to load functions, there's nothing we can do */
    if (errorFunctions)
        return <ErrorFunctions />;

    const {hasOutput, defaultPanelSize} = state;    
    let size = defaultPanelSize;
    
    if (!hasOutput)
        size = "100%";

    return (
        <>            
            {
                state.spinnerText && 
                    <Spinner size="large" label={state.spinnerText} fullScreen={true}  />
            } 
            <SplitPane 
                ref={splitPaneRef}
                style={{ display: state.spinnerText ? "none" : "flex", position: undefined }} 
                split="horizontal" 
                minSize={200} 
                size={size}
                defaultSize={state.defaultPanelSize}                
                onChange={debounce((size) => dispatch({ type: "SPLIT_RESIZE", defaultPanelSize: size }), 150)}
            >                
                <div 
                    id="admin-tools-fiddle" 
                    data-testid="admin-tools-fiddle" 
                    style={{ width: "100%", display: "flex", flexDirection: "column" }}
                >
                    <FiddleToolbar 
                        storage={storage}
                        contextTypes={contextTypes}
                        input={input}
                        inputFormat={activeTabInputFormat}
                        onChangeInputFormat={format => {
                            dispatch({ type: "CHANGE_FORMAT", format });
                        }}
                        selectedFlags={state.selectedFlags}
                        setSelectedFlags={selectedFlags => dispatch({ type: "SET_SELECTED_FLAGS", selectedFlags })}
                        selectedEditor={state.selectedEditor}
                        onSelectEditor={selectedEditor => dispatch({ type: "SET_SELECTED_EDITOR", selectedEditor })}
                        inputLabel={activeTabLabel}                        
                        onNewFile={onNewFile}                        
                        onOpenRecent={onOpenRecentFile}
                        onSaveFile={onSaveFile}
                        onSaveFileAs={onSaveFileAs}
                        onOpen={onOpen}
                        canDelete={Boolean(activeTabContent && activeTabContent.label)}
                        onDeleteScript={onDeleteScript}                        
                        canList={(input && input.length > 0)}
                        onList={onList}
                        canEvaluate={(input && input.length > 0)}
                        onEvaluate={onEvaluate}                                                                  
                        contextVariables={state.contextVariables}
                        setContextVariables={contextVariables => dispatch({ type: "SET_CONTEXT_VARIABLES", contextVariables })}    
                        onTabClose={() => onTabClose(state.activeTab)}             
                        onTabCloseAll={onTabCloseAll}                  
                        setSelectedContextTypes={selectedContextTypes => dispatch({ type: "SET_SELECTED_CONTEXT_TYPES", selectedContextTypes })}
                    />                                       
                    <QualifiersProvider contextTypes={state.selectedContextTypes}>
                        <div style={{ flex: 1, height: "calc(100% - 64px)", width: "100%", marginBottom: theme.spacing(0.5), marginTop: theme.spacing(0.5) }}>
                            {
                                (state.selectedEditor === "source") ?     
                                    <TabbedCodeEditor 
                                        ref={editorRef}
                                        // eslint-disable-next-line jsx-a11y/no-autofocus
                                        autoFocus={true}
                                        tabs={state.tabs}
                                        activeTab={state.activeTab}
                                        onNewTab={onNewFile}
                                        onTabSwitch={(activeTab) => dispatch({ type: "TAB_SWITCH", activeTab })}
                                        onSourceChanged={(newValue) => {
                                            dispatch({ type: "SOURCE_CHANGED", newValue });
                                        }}
                                        onTabClose={onTabClose}
                                        onSaveSource={onSaveFile}
                                        additionalActionMenuOptions={[
                                            {
                                                id: "copy_action",
                                                label: "Copy Action Request",
                                                contextMenuGroupId: "copy",
                                                contextMenuOrder: 1.1,
                                                run: onCopyAction
                                            },
                                        ]}
                                    />
                                    :
                                    <OperationalContext.Provider value={OperationalMode.Editable}>
                                        <HybridBuilder                                    
                                            hybrid={hybrid ? JSON.parse(hybrid) : undefined}
                                            onChanged={hybrid => setHybrid(JSON.stringify(hybrid))}
                                        />
                                    </OperationalContext.Provider>                                
                            }
                        </div>
                    </QualifiersProvider>         
                </div>                       
                <EvaluationOutput 
                    format={state.evaluatedFormat}
                    subformat={state.evaluatedSubformat}
                    size={state.defaultPanelSize}
                    outputPanelSize={state.outputPanelSize}
                    error={state.error}                    
                    result={state.result}
                    resultDataType={state.resultDataType}
                    listing={state.listing}
                    stdout={state.stdout}
                    stderr={state.stderr}
                    onClear={() => {
                        dispatch({ type: "CLOSE_OUTPUT" });                        
                    }}
                /> 
            </SplitPane>  
            <ConfirmDeleteDialog 
                open={Boolean(state.showConfirmDelete)}
                label={activeTabLabel}
                onDismiss={() => dispatch({ type: "DISMISS_DELETE" })}
                onConfirm={onConfirmDelete}
            />      
            <ConfirmCloseDialog 
                open={Boolean(state.showConfirmClose)}
                label={activeTabLabel}
                onDismiss={() => dispatch({ type: "DISMISS_CLOSE" })}
                onConfirm={() => onTabCloseConfirm()}
            />      
            <ConfirmCloseAllDialog 
                open={Boolean(state.showConfirmCloseAll)}
                onDismiss={() => dispatch({ type: "DISMISS_CLOSE_ALL" })}
                onConfirm={() => onTabCloseAllConfirm()}
            />        
            <OpenFileDialog 
                open={Boolean(state.showOpen)}
                onDismiss={() => dispatch({ type: "DISMISS_FILE_OPEN" })}
                vfsAdaptors={vfsAdaptors}
                storage={storage}
                onOpen={onOpenFile}
            />
            <SaveFileDialog 
                open={Boolean(state.showSaveAs)}
                onDismiss={() => dispatch({ type: "DISMISS_SAVE_AS" })}
                vfsAdaptors={vfsAdaptors}
                storage={storage}
                onSaveLocal={(fileName) => {
                    onSaveLocalFile(fileName);
                }}
                onSaveVfs={(fileName, node) => {                                        
                    if (node.isDirectory)
                        onCreateVfsFile(fileName, node.path);
                    else
                        onSaveVfsFile(fileName, node.path);                    
                }}
            />
            <Prompt 
                when={state.dirty ? true : false}
                message={() => "You have unsaved changes.  Are you sure you want to leave?"}
            />   
        </>
    );
};

export default Fiddle;
