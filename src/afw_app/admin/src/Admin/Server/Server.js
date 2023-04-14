// See the 'COPYING' file in the project root for licensing information.

import {useState} from "react";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {useBreadcrumbs, useTheme} from "../../hooks";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";

import {
    Breadcrumb,
    ObjectResponsive,
    Tabs,
    Typography,
    useSystemInfo,
    useServer
} from "@afw/react";

const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

/**
 * Server displays a composite layout for /afw/_AdaptiveServer_/current and 
 * /afw/_AdaptiveSystemInfo_/.  It's rendered under the /Admin/Server url.
 */
export const Server = () => {

    const [showHelp, setShowHelp] = useState(false);

    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);
    const theme = useTheme();
    const {systemInfo} = useSystemInfo();
    const {server} = useServer();        
    
    /**
     * Sort the systemInfo objects into Tabs with "General" coming first.
     */
    const sortedSystemInfo = systemInfo ?        
        systemInfo.sort((a, b) => {
            const a_objectId = a._meta_.objectId;
            const b_objectId = b._meta_.objectId;
            
            if (a_objectId === "General")
                return -1;
            else if (b_objectId === "General")
                return 1;
            else 
                return (a_objectId.toLowerCase().localeCompare(b_objectId.toLowerCase()));
        }) : [];    

    return (
        <div id="admin-admin-server" data-testid="admin-admin-server" style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={setShowHelp} />                
            </div>
            <div style={{ flex: 1, overflow: "auto" }}>
                <div style={{ padding: theme.spacing(2) }}>       
                    <Typography component="h2" size="8" text="Server Information" />      
                    <div style={{ height: theme.spacing(2) }} />
                    <ObjectResponsive 
                        object={server}
                        spacing={theme.spacing(1)}
                    />
                </div>                
                <div style={{ height: theme.spacing(5) }} />                
                <div style={{ padding: theme.spacing(2) }}>
                    <Typography component="h2" size="8" text="System Information" />
                    <div style={{ height: theme.spacing(2) }} />
                    <Tabs 
                        gapSpace={16}
                        tabs={
                            sortedSystemInfo.map(sysInfo => ({
                                text: sysInfo._meta_.objectId,
                                contains: 
                                    <ObjectResponsive 
                                        object={sysInfo}
                                        spacing={theme.spacing(1)}
                                        filterOptions={{
                                            filterValuedProperties: true,
                                        }}
                                    />
                            }))
                        }
                    />
                </div>                
            </div> 
            <ContextualHelp 
                open={showHelp}
                onClose={() => setShowHelp(false)}
                routes={ContextualHelpRoutes}
            /> 
        </div>                    
    );    
};

export default Server;
