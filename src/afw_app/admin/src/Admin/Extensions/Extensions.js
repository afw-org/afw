// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useMemo, useReducer} from "react";


import {
    Breadcrumb,
    Button,
    Checkbox,
    Dialog,
    ObjectResponsive,
    Spinner,
    Table,
    TextField,
    Typography,
    useModel,
    useExtensionManifests,
    useExtensions,
    useIsMounted,
    useFunctions,
} from "@afw/react";

import {useApplication, useAppCore, useBreadcrumbs, useTheme} from "../../hooks";
import {afwExtensionLoad, afwExtensionLoadByModulePath} from "@afw/core";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";

/**
 * Opens up a dialog to Add  new extension, prompting for an extensionId and modulePath 
 */
const AddExtensionDialog = ({ open, onDismiss, onAdd }) => {

    const theme = useTheme();
    const [modulePath, setModulePath] = useState("");
    const {functions} = useFunctions();
    const extensionLoadDescription = useMemo(() => {
        if (functions) {
            for (const f of functions) {
                if (f.functionId === "extension_load_by_module_path") {                    
                    return f.parameters[0].description;
                }
            }
        }
    }, [functions]);

    const dismiss = () => {        
        setModulePath();
        onDismiss();
    };

    const add = () => {        
        onAdd(modulePath);        
        setModulePath();
    };

    return(
        <Dialog 
            open={open}
            title="Add Extension"
            isBlocking={true}
            onDismiss={dismiss}
            showClose={true}
            maxWidth="md"
            contains={
                <div>
                    <div style={{ padding: theme.spacing(2) }}>                        
                        <TextField                             
                            id="add-extension-module-path"
                            // eslint-disable-next-line jsx-a11y/no-autofocus
                            autoFocus={true}
                            aria-label="Module Path"
                            label="Module Path"
                            value={modulePath}
                            required                            
                            description={extensionLoadDescription}
                            onChanged={setModulePath}
                        />
                    </div>
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2) }}>
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            label="Cancel"
                            onClick={dismiss}
                        />
                        <Button 
                            label="Add"
                            color="primary"
                            variant="contained"
                            disabled={!modulePath}
                            onClick={add}
                        />
                    </div>
                </div>
            }
        />
    );
};

/**
 * Opens up a Dialog to confirm loading an extension and asking if the extension should
 * be loaded on application startup.
 */
const ConfirmDialog = ({ open, onDismiss, onLoadExtensions }) => {

    const theme = useTheme();
    const [onAppStartup, setOnAppStartup] = useState(false);

    return (
        <Dialog 
            open={open}
            title="Load Extension"
            subText="Load the selected extension?"
            isBlocking={false}
            showClose={true}
            onDismiss={onDismiss}
            contains={                                      
                <div style={{ paddingRight: theme.spacing(0.5), verticalAlign: "middle" }}>
                    <Checkbox                         
                        onChanged={setOnAppStartup}
                        label="Load on startup"
                        value={onAppStartup}
                    />
                    <div style={{ height: theme.spacing(2) }} />
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", width: "100%" }}>
                    <Button     
                        style={{ marginRight: theme.spacing(1) }}       
                        variant="contained"
                        label="Yes"
                        onClick={() => onLoadExtensions(onAppStartup)}
                    />
                    <Button                            
                        label="No"
                        onClick={onDismiss}
                    />                            
                </div>                    
            }
        />
    );
};

const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

const initialState = {
    showConfirmDialog:      false,
    showAddExtension:       false,
    showExtensionDetails:   false,
    selectedManifests:      [],
    isLoading:              false,
    showHelp:               false,
};

const reducer = (state, action) => {

    switch(action.type) {

    case "EXTENSION_ADD_CLICK":
        return {
            ...state,
            showAddExtension: true,
        };

    case "EXTENSION_ADD":
        return {
            ...state,
            showAddExtension: false,            
        };

    case "EXTENSION_ADDING":
        return {
            ...state,
            isLoading: true,
        };

    case "EXTENSION_ADDED":
        return {
            ...state,
            isLoading: false,
        };

    case "EXTENSION_ADD_DISMISS":
        return {
            ...state,
            showAddExtension: false,
        };

    case "EXTENSION_LOAD":
        return {
            ...state,
            showConfirmDialog: true,
        };

    case "EXTENSIONS_LOADING":
        return {
            ...state,
            isLoading: true,
        };

    case "EXTENSIONS_LOADED":
        return {
            ...state,
            isLoading: false,
            selectedManifests: [],
            showConfirmDialog: false,            
        };

    case "EXTENSION_LOAD_DISMISS":
        return {
            ...state,
            showConfirmDialog: false,
        };

    case "EXTENSION_INVOKED":
        return {
            ...state,
            selectedManifest: action.selectedManifest,
            showExtensionDetails: true,
        };

    case "EXTENSION_DETAILS_DISMISS":
        return {
            ...state,
            showExtensionDetails: false,
        };        

    case "SELECT_MANIFESTS":
        return {
            ...state,
            selectedManifests: action.selectedManifests,
        };

    case "LOADING_CONFIGURATION":
        return {
            ...state,
            isLoadingConfiguration: true,
        };

    case "APPLICATION_CONF_LOADED":
        return {
            ...state,
            isLoadingConfiguration: false,
            applicationConf: action.applicationConf,
        };

    case "ERROR":
        return {
            ...state,
            error: action.error,
            isLoading: false,
            showConfirmDialog: false,
            isLoadingConfiguration: false,
        };

    case "HELP_CLICK":
        return {
            ...state,
            showHelp: true,
        };        

    case "HELP_DISMISS":
        return {
            ...state,
            showHelp: false,
        };        

    default:
        return state;

    }
};


/**
 * Main Extensions component that renders the /Admin/Extensions page.
 * 
 * This page initially renders a table of the extensions available through 
 * the manifest (/afw/_AdaptiveManifest_/) objects.  Each row in the table
 * can be selected to view its details.  Extensions that are not loaded
 * can be with the "Load Extension" button.  New extensions, not available 
 * in the manifest, can be added using the "Add Extension" button.
 */
const Extensions = () => {    
    
    const [state, dispatch] = useReducer(reducer, initialState);

    const {
        showConfirmDialog,
        showAddExtension,
        isLoading,
        showExtensionDetails,
        selectedManifest,
        selectedManifests,
        isLoadingConfiguration,
        applicationConf,
        showHelp,
    } = state;    
        
    const {extensions, isLoading: isLoadingExtensions} = useExtensions();
    const {manifests, onRefresh: reloadManifests, isLoading: isLoadingManifests} = useExtensionManifests();
    const {notification, client} = useApplication();
    const {application, reloadApp} = useAppCore();
    const model = useModel();
    const theme = useTheme();
    const isMounted = useIsMounted();
    
    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);

    const extensionsMap = useMemo(() => {
        if (extensions) {
            return Object.assign( ...extensions.map(i => ({[i.extensionId]: i})) );
        } else return [];
    }, [extensions]);

    /* sort the manifest objects from those that are loaded, followed by those that are not.  Then, sort alphabetically */
    const sortedManifests = useMemo(() => {
        if (manifests && extensionsMap) {
            /* sort loaded above not loaded */
            let sortedManifests = manifests.sort((a, b) => {                
                let A = a.getPropertyValue("extensionId");
                let B = b.getPropertyValue("extensionId");

                if (extensionsMap[A] && !extensionsMap[B])
                    return -1;
                else if (extensionsMap[B] && !extensionsMap[A])
                    return 1;
                else 
                    return A.toLowerCase().localeCompare(B.toLowerCase());
            });        

            return sortedManifests;
        } else return [];
    }, [manifests, extensionsMap]);

    /* once we have our application object, loaded extensions, and manifests, go ahead and fetch our _AdaptiveConf_application object */
    useEffect(() => {
        let controller;

        /**
         * This routine examines both the extensions and extension manifests objects to create a table 
         *  of extensions, some loaded, some that haven't been loaded.  The manifests shows all extensions, but the 
         *  extensions shows only ones that have been loaded.  The loaded are necessary to pull out version information.
         */
        const loadConfig = async () => {
            dispatch({ type: "LOADING_CONFIGURATION" });

            try {
                const {applicationId, confAdapterId} = application.getPropertyValues();    
                const response = model.getObject({
                    objectTypeId: "_AdaptiveConf_application", 
                    objectId: applicationId, 
                    adapterId: confAdapterId
                });
                controller = response.controller;

                const applicationConf = await response.object;      
                if (!controller.signal.aborted)
                    dispatch({ type: "APPLICATION_CONF_LOADED", applicationConf });
                    

            } catch (error) {
                if (!controller.signal.aborted) {
                    dispatch({ type: "ERROR", error });
                    notification({ message: error, type: "error" });
                }
            }            
        };

        if (application && extensions && manifests) {            
            loadConfig();
        }

        return () => {
            if (controller)
                controller.abort();
        };

    }, [application, extensions, manifests, model, notification]);


    const onAddExtension = async (modulePath) => {
        
        dispatch({ type: "EXTENSION_ADDING" });

        try {
            const extensionId = await afwExtensionLoadByModulePath(client, modulePath).result();
            if (!isMounted()) return;

            /* reload application and functions that may have been updated by this extension load */            
            await reloadApp();
            if (!isMounted()) return;

            await reloadManifests();

            dispatch({ type: "EXTENSION_ADDED" });
            /* let user know it was loaded and refreshed */
            notification({ message: "Extension " + extensionId + " was loaded successfully.", type: "success" });
        } catch (error) {
            dispatch({ type: "ERROR", error });
            notification({ message: error, type: "error" });
        }      
    };
    
    const onLoadExtensions = async (onAppStartup) => {
        
        dispatch({ type: "EXTENSIONS_LOADING" });

        try {           
            for (const manifest of selectedManifests) {
                await afwExtensionLoad(client, manifest.getPropertyValue("extensionId")).result();
                if (!isMounted()) return;
            }

            if (onAppStartup) {                
                let extensions = applicationConf.getPropertyValue("extensions");
                if (!extensions)
                    extensions = applicationConf.getProperty("extensions").setValue([]);

                selectedManifests.forEach(manifest => {                    
                    if (!extensions.includes(manifest.getPropertyValue("extensionId"))) {
                        let extension = extensions.newValue();
                        extension.setValue(manifest.getPropertyValue("extensionId"));
                    }
                });

                /* get a fresh copy */
                await applicationConf.refresh({ reconcilable: true });
                if (!isMounted()) return;
                applicationConf.setPropertyValue("extensions", extensions);            
                
                const response = applicationConf.reconcile();
                await response.result();                
                if (!isMounted()) return;
                
                /* reload application and functions that may have been updated by this extension load */
                await reloadApp();
                if (!isMounted()) return;
                await reloadManifests();
                if (!isMounted()) return;                
            } else {
                /* reload application and functions that may have been updated by this extension load */
                await reloadApp();
                if (!isMounted()) return;
                await reloadManifests();
                if (!isMounted()) return;
            }

            notification({ message: "Extensions loaded and application refreshed.", type: "success", duration: 3000 });
            dispatch({ type: "EXTENSIONS_LOADED" });
        } catch (error) {            
            notification({ message: error, type: "error" });
            dispatch({ type: "ERROR", error });
        }
    };
    
    const onRefresh = async () => {
        /* simply reload configuration props */
        await reloadManifests();
    };

    let canLoadExtension = false;    
    if (selectedManifests && selectedManifests.length > 0) {        
        canLoadExtension = true;
        for (const selectedManifest of selectedManifests) {            
            if (extensions.some(ext => ext.extensionId === selectedManifest.getPropertyValue("extensionId"))) {                
                canLoadExtension = false;
            }
        }        
    }
    
    return (
        <div id="admin-admin-extensions" style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={() => dispatch({ type: "HELP_CLICK" })} />                
            </div>
            {
                (isLoading || isLoadingConfiguration || isLoadingExtensions || isLoadingManifests) &&   
                    <Spinner size="large" label="Loading Extension Configurations..." fullScreen={true} />
            }
            {
                sortedManifests &&
                <div style={{ flex: 1, overflow: "auto" }}>
                    <Table 
                        compact={false}
                        rows={sortedManifests}
                        columns={[                                                
                            { 
                                key: "extensionId", name: "Extension Id", isResizable: true, minWidth: 200, maxWidth: 200,
                                onRender: (manifest) => {
                                    const extensionId = manifest.getPropertyValue("extensionId");

                                    if (extensionsMap[extensionId])
                                        return <Typography text={extensionId} />;
                                    else
                                        return <Typography color="textSecondary" text={extensionId} />;
                                }
                            },
                            { 
                                key: "extensionVersion", name: "Version", isResizable: true, minWidth: 200, maxWidth: 200, 
                                isMultiline: true, style: { whiteSpace: "pre-line" },
                                onRender: (manifest) => {                                    
                                    const extensionId = manifest.getPropertyValue("extensionId");
                                    const loadedExtension = extensionsMap[extensionId];

                                    if (loadedExtension) {
                                        const {extensionVersion, afwCompiledVersion} = loadedExtension;
                                        return <Typography text={extensionVersion + "\n" + afwCompiledVersion} />;
                                    }
                                    else 
                                        return null;
                                } 
                            },                                                  
                            { 
                                key: "description", name: "Description", isResizable: true, minWidth: 400, isMultiline: true,
                                style: { wordWrap: "break-word" },
                                onRender: (manifest) => {
                                    const extensionId = manifest.getPropertyValue("extensionId");

                                    if (extensionsMap[extensionId])
                                        return <Typography text={manifest.getPropertyValue("brief")} />;
                                    else 
                                        return <Typography color="textSecondary" text={manifest.getPropertyValue("brief")} />;
                                }
                            },
                        ]}
                        selectionMode="multiple"
                        onSelectionChanged={selectedManifests => dispatch({ type: "SELECT_MANIFESTS", selectedManifests })}                        
                        onRowInvoked={(selectedManifest) => dispatch({ type: "MANIFEST_INVOKED", selectedManifest })}
                    />                    
                    <ConfirmDialog 
                        open={showConfirmDialog}                        
                        onDismiss={() => dispatch({ type: "EXTENSION_LOAD_DISMISS" })}
                        onLoadExtensions={onLoadExtensions}
                    />
                    <Dialog 
                        open={showExtensionDetails}
                        title="Extension Details"
                        isBlocking={false}
                        onDismiss={() => dispatch({ type: "EXTENSION_DETAILS_DISMISS" })}
                        showClose={true}
                        maxWidth="md"
                        contains={
                            <div>
                                <ObjectResponsive 
                                    object={selectedManifest}
                                    editable={false}
                                />
                            </div>
                        }
                    />   
                    <AddExtensionDialog 
                        open={showAddExtension}                        
                        onDismiss={() => dispatch({ type: "EXTENSION_ADD_DISMISS" })}
                        onAdd={(extensionId, modulePath) => {                            
                            onAddExtension(extensionId, modulePath);

                            dispatch({ type: "EXTENSION_ADD" });
                        }}
                    />
                </div>                
            }
            <div style={{ padding: theme.spacing(2), display: "flex", justifyContent: "space-between" }}>
                <div style={{ display: "flex" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            variant="contained"
                            color="primary"
                            icon="power_settings_new_icon"
                            label="Load Extension"
                            disabled={!canLoadExtension}
                            onClick={() => dispatch({ type: "EXTENSION_LOAD" })}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button                                                      
                            label="Refresh"                
                            icon="refresh"            
                            onClick={onRefresh}
                        />
                    </div>
                    <div>
                        <Button                                                      
                            label="Details"       
                            icon="launch"                     
                            onClick={() => dispatch({ type: "EXTENSION_INVOKED", selectedManifest: selectedManifests[0] })}
                            disabled={(selectedManifests.length !== 1)}
                        />
                    </div>
                </div>
                <div>
                    <Button 
                        label="Add Extension"
                        icon="add"         
                        onClick={() => dispatch({ type: "EXTENSION_ADD_CLICK" })}                       
                    />
                </div>
            </div>
            <ContextualHelp 
                open={showHelp}
                onClose={() => dispatch({ type: "HELP_DISMISS" })}
                routes={ContextualHelpRoutes}
                manifests={sortedManifests}
            /> 
        </div>
    );
};

export default Extensions;
