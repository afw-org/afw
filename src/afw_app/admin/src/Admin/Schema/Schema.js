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
 * items, then displays a Table of adaptors to select from.  Object 
 * Type requests are routed to ObjectTypes component.
 */
const Schema = () => {

    const [breadcrumbItems, setBreadcrumbItems] = useState([]);
    const [showHelp, setShowHelp] = useState(false);
    const [adaptorId, setAdaptorId] = useState();
    const [adaptor, setAdaptor] = useState();
    const [error, setError] = useState();

    const theme = useTheme();
    const {pathname} = useLocation();
    const {adaptors} = useAppCore();

    /* parse the pathname and break it into Breadcrumbs */
    useEffect(() => {
        let [, adaptorId, objectType, propertyType] = pathname.split("/").splice(2); // eslint-disable-line

        let breadcrumbItems = [
            { text: "Admin", key: "Admin", link: "/Admin" },
            { text: "Schema", key: "Schema", link: "/Admin/Schema" }
        ];

        /* construct our breadcrumb trail from the matching path */
        if (adaptorId)
            breadcrumbItems.push({ 
                text: adaptorId, key: adaptorId, 
                link: "/Admin/Schema/" + adaptorId 
            });

        if (objectType) 
            breadcrumbItems.push({ 
                text: objectType, key: objectType, 
                link: "/Admin/Schema/" + adaptorId + "/" + objectType 
            });

        if (propertyType) 
            breadcrumbItems.push({ 
                text: propertyType, key: propertyType, 
                link: "/Admin/Schema/" + adaptorId + "/" + objectType + "/" + propertyType
            });

        setAdaptorId(adaptorId);
        setBreadcrumbItems(breadcrumbItems);
    }, [pathname]);

    useEffect(() => {
        if (adaptors && adaptorId) {
            let found = false;
            adaptors.forEach(a => {
                if (a.adaptorId === adaptorId) {
                    setAdaptor(a);
                    found = true;
                }
            });

            if (!found)
                setError("AdaptorId not found");
        }
    }, [adaptorId, adaptors]);

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
                            rows={adaptors ? adaptors : []}
                            columns={[
                                {
                                    key: "adaptorId", 
                                    name: "Adaptor Id", 
                                    minWidth: 100, 
                                    maxWidth: 200,
                                    isResizable: true,
                                    width: "20%", 
                                    onRender: adaptor => {
                                        const adaptorId = adaptor.adaptorId;
                                        return <Link text={adaptorId} uriComponents={["Admin", "Schema", adaptorId]} />;
                                    
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
                                    onRender: adaptor => adaptor.properties.description
                                }
                            ]}
                            selectionMode="none"
                        />
                    } />

                    <Route path="/Admin/Schema/:adaptorId" render={() => {
                        if (adaptorId && adaptor) {
                            return (
                                <ObjectTypes 
                                    adaptorId={adaptorId}
                                    adaptor={adaptor}                            
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
