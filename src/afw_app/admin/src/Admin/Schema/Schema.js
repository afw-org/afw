// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {Route, Switch, useLocation} from "react-router";

import {
    Breadcrumb,
    Link,
    Table,
    Typography
} from "@afw/react";

import {useAppCore, useTheme} from "../../hooks";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";

import {ObjectTypes} from "./ObjectTypes";
import {ContextualHelpRoutes} from "./ContextualHelp";

/*
 * Schema
 *
 * Main component for routing Schema parts of the App.  It 
 * parses the React Router pathname and generates the Breadcrumb
 * items, then displays a Table of adapters to select from.  Object 
 * Type requests are routed to ObjectTypes component.
 */
const Schema = () => {

    const [breadcrumbItems, setBreadcrumbItems] = useState([]);
    const [showHelp, setShowHelp] = useState(false);
    const [adapterId, setAdapterId] = useState();
    const [adapter, setAdapter] = useState();
    const [error, setError] = useState();

    const theme = useTheme();
    const {pathname} = useLocation();
    const {adapters} = useAppCore();

    /* parse the pathname and break it into Breadcrumbs */
    useEffect(() => {
        let [, adapterId, objectType, propertyType] = pathname.split("/").splice(2); // eslint-disable-line

        let breadcrumbItems = [
            { text: "Admin", key: "Admin", link: "/Admin" },
            { text: "Schema", key: "Schema", link: "/Admin/Schema" }
        ];

        /* construct our breadcrumb trail from the matching path */
        if (adapterId)
            breadcrumbItems.push({ 
                text: adapterId, key: adapterId, 
                link: "/Admin/Schema/" + adapterId 
            });

        if (objectType) 
            breadcrumbItems.push({ 
                text: objectType, key: objectType, 
                link: "/Admin/Schema/" + adapterId + "/" + objectType 
            });

        if (propertyType) 
            breadcrumbItems.push({ 
                text: propertyType, key: propertyType, 
                link: "/Admin/Schema/" + adapterId + "/" + objectType + "/" + propertyType
            });

        setAdapterId(adapterId);
        setBreadcrumbItems(breadcrumbItems);
    }, [pathname]);

    useEffect(() => {
        if (adapters && adapterId) {
            let found = false;
            adapters.forEach(a => {
                if (a.adapterId === adapterId) {
                    setAdapter(a);
                    found = true;
                }
            });

            if (!found)
                setError("AdapterId not found");
        }
    }, [adapterId, adapters]);

    /* Report any errors */
    if (error) {
        return <Typography text={error} />;   
    }

    return (
        <div id="admin-admin-schema" data-testid="admin-admin-schema" style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb style={{ display: "inline-block" }} items={breadcrumbItems} />  
                </div> 
                <div style={{ display: "inline-block" }}>
                    <ContextualHelpButton showHelp={setShowHelp} />
                </div>                     
            </div>
            <div style={{ flex: 1, overflow: "auto" }}>
                <Switch>
                    <Route exact path="/Admin/Schema/" render={() => 
                        <Table 
                            rows={adapters ? adapters : []}
                            columns={[
                                {
                                    key: "adapterId", 
                                    name: "Adapter Id", 
                                    minWidth: 100, 
                                    maxWidth: 200,
                                    isResizable: true,
                                    width: "20%", 
                                    onRender: adapter => {
                                        const adapterId = adapter.adapterId;
                                        return <Link text={adapterId} uriComponents={["Admin", "Schema", adapterId]} />;
                                    
                                    }
                                },
                                {
                                    key: "description", 
                                    name: "Description", 
                                    isResizable: true, 
                                    isMultiline: true,
                                    minWidth: 100, 
                                    maxWidth: 400,
                                    width: "80%", 
                                    onRender: adapter => adapter.properties.description
                                }
                            ]}
                            selectionMode="none"
                        />
                    } />

                    <Route path="/Admin/Schema/:adapterId" render={() => {
                        if (adapterId && adapter) {
                            return (
                                <ObjectTypes 
                                    adapterId={adapterId}
                                    adapter={adapter}                            
                                />
                            );
                        } else return null;
                    }} />
                </Switch> 
            </div>
            <ContextualHelp 
                open={showHelp}
                onClose={() => setShowHelp(false)}
                routes={ContextualHelpRoutes}
            /> 
        </div>
    );
};

export default Schema;
