// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Breadcrumb,
    ObjectEditor,
    ObjectResponsive,
    PropertyResponsive,
    Spinner,    
    Tabs,
    Typography,
    useGetObject,
    useOperational,
    useValues,
    objectOptions_reconcilable,
} from "@afw/react";

import {
    useApplication, 
    useAppCore, 
    useBreadcrumbs, 
    useTheme
} from "../../hooks";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";

/**
 * The ApplicationLayout component is what gets rendered by the ObjectEditor
 * component.  This layout divides up the Application object into tabs:
 * 
 *   General, Startup, Flags, Authorization, Variables
 * 
 */
const ApplicationLayout = ({ applicationConf, selectedTab, setSelectedTab }) => {
    
    const {editable} = useOperational();   
    const theme = useTheme();    

    return (        
        <div style={{ padding: theme.spacing(2) }}>
            <Tabs 
                selectedTab={selectedTab}
                onTabSwitch={setSelectedTab}
                gapSpace={16}
                tabs={[
                    {
                        text: "General",
                        contains: 
                            <ObjectResponsive
                                filterOptions={{
                                    filterProperties: [
                                        "title",
                                        "description",
                                        "applicationId", 
                                        "confAdaptorId", 
                                        "defaultAdaptorId", 
                                        "defaultModelAdaptorId", 
                                        "layoutsAdaptorId",
                                        "rootFilePaths"
                                    ],
                                    filterValuedProperties: editable ? false : true
                                }}
                                breakpoints={{
                                    large: 9,
                                    xl: 6,
                                }}
                            />
                    },                            
                    {
                        text: "Startup",
                        contains: 
                            <ObjectResponsive 
                                filterOptions={{
                                    filterProperties: [ "extensions", "extensionModulePaths", "onApplicationStartupComplete" ],
                                    filterValuedProperties: editable ? false : true
                                }}
                                breakpoints={{
                                    large: 9,
                                }}
                            />
                    },
                    {
                        text: "Flags",
                        contains: 
                            (!editable && (applicationConf && applicationConf.getPropertyValue("defaultFlags") === undefined)) ?
                                <div style={{ padding: theme.spacing(1) }}>
                                    <Typography text="No Default Flags have been defined." size="3" />
                                </div> 
                                :
                                <ObjectResponsive 
                                    filterOptions={{
                                        filterProperties: [ "defaultFlags" ]
                                    }}
                                    breakpoints={{
                                        large: 9,
                                    }}
                                />
                    },
                    {
                        text: "Authorization",
                        contains:
                            (!editable && (applicationConf && applicationConf.getPropertyValue("authorizationControl") === undefined)) ?
                                <div style={{ padding: theme.spacing(1) }}>
                                    <Typography text="No Properties defined for Authorization Control" size="3" />
                                </div>
                                :
                                <PropertyResponsive 
                                    property={applicationConf ? applicationConf.getProperty("authorizationControl") : undefined}
                                    filterOptions={{
                                        filterValuedProperties: editable ? false : true,
                                    }}
                                    breakpoints={{
                                        large: 9,
                                    }}
                                />
                                
                    },
                    {
                        text: "Variables",
                        contains: 
                            (!editable && (applicationConf && applicationConf.getPropertyValue("qualifiedVariables") === undefined)) ?
                                <div style={{ padding: theme.spacing(1) }}>
                                    <Typography text="No Qualified Variables have been defined." size="3" />
                                </div> 
                                :
                                <ObjectResponsive            
                                    filterOptions={{
                                        filterProperties: [ "qualifiedVariables" ]
                                    }}
                                    isCollapsed={false}
                                    embedded={{ component: "None" }}
                                    breakpoints={{
                                        large: 9,
                                    }}
                                    spacing={theme.spacing(0.5)}
                                />
                    }
                ]}
            />
        </div>          
    );

};

const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

/**
 * The Application component displays the /afw/_AdaptiveApplication/<app> object
 * that's located under the /Admin/Application route. 
 */
export const Application = () => {

    const [showHelp, setShowHelp] = useState(false);
    const [selectedTab, setSelectedTab] = useState();

    const theme = useTheme();    
    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);
    const {notification} = useApplication();
    const {application} = useAppCore();
    const {applicationId, confAdaptorId} = useValues(application);
    const {object: applicationConf, isLoading, error} = useGetObject({ 
        adaptorId: confAdaptorId, objectTypeId: "_AdaptiveConf_application",
        objectId: applicationId, objectOptions: objectOptions_reconcilable
    });    

    useEffect(() => {
        if (error)
            notification({ message: error, type: "error" });
    }, [error, notification]);
        
    return (        
        <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={setShowHelp} />                
            </div>            
            {
                isLoading && 
                    <Spinner size="large" label="Loading Application Configuration" fullScreen={true} />
            }
            <div style={{ flex: 1, overflow: "auto" }}>                
                <ObjectEditor 
                    object={applicationConf}                    
                    layout={
                        <ApplicationLayout 
                            applicationConf={applicationConf}                            
                            selectedTab={selectedTab}
                            setSelectedTab={(tab, selectedTab) => setSelectedTab(selectedTab)}
                        />
                    }
                    onSaved={() => notification({ message: "Application object saved.", type: "success" })}
                />                                 
            </div>
            <ContextualHelp 
                applicationConf={applicationConf}
                isLoading={isLoading}
                error={error}
                open={showHelp}
                onClose={() => setShowHelp(false)}
                routes={ContextualHelpRoutes}
            /> 
        </div>
    );             
};

export default Application;
