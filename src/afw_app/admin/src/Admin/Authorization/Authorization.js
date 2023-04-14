// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Breadcrumb,
} from "@afw/react";

import {useBreadcrumbs, useTheme} from "../../hooks";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";
import {ContextualHelpRoutes} from "./ContextualHelp";

const root = { text: "Admin", link: "/Admin" };

export const Authorization = () => {
    
    const theme = useTheme();
    const [showHelp, setShowHelp] = useState(false);
    const breadcrumbItems = useBreadcrumbs(root);    

    return (
        <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={setShowHelp} />                
            </div>
            <div style={{ flex: 1, overflow: "auto" }}>
            </div>
            <ContextualHelp 
                open={showHelp}
                onClose={() => setShowHelp(false)}
                routes={ContextualHelpRoutes}
            /> 
        </div>
    );
};

export default Authorization;
