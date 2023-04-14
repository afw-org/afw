// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Breadcrumb,
} from "@afw/react";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {useBreadcrumbs, useTheme} from "../../hooks";
import {ContextualHelp, ContextualHelpButton} from "../../common/ContextualHelp";

const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

const Provisioning = () => {

    const theme = useTheme();
    const [showHelp, setShowHelp] = useState(false);
    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);

    return (
        <div id="admin-admin-provisioning" data-testid="admin-admin-provisioning" style={{ display: "flex", flexDirection: "column", height: "100%" }}>
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

export default Provisioning;
