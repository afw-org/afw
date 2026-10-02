// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";
import {useParams} from "@tanstack/react-router";

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
 * reads the route's params and generates the Breadcrumb
 * items, then displays a Table of adapters to select from.  Object 
 * Type requests are routed to ObjectTypes component.
 */
const Schema = () => {

    const [showHelp, setShowHelp] = useState(false);

    const theme = useTheme();
    const {adapters} = useAppCore();

    /* the route's optional params: adapter > object type > property */
    const {adapterId, objectTypeId, propertyName} = useParams({ strict: false });

    /* break them into Breadcrumbs */
    const breadcrumbItems = useMemo(() => {
        const items = [
            { text: "Admin", key: "Admin", link: "/Admin" },
            { text: "Schema", key: "Schema", link: "/Admin/Schema" }
        ];

        let link = "/Admin/Schema";
        for (const segment of [adapterId, objectTypeId, propertyName]) {
            if (!segment)
                break;
            link += "/" + encodeURIComponent(segment);
            items.push({ text: segment, key: segment, link });
        }

        return items;
    }, [adapterId, objectTypeId, propertyName]);

    const {adapter, error} = useMemo(() => {
        if (adapters && adapterId) {
            const found = adapters.find(a => a.adapterId === adapterId);
            if (found)
                return {adapter: found, error: undefined};
            else
                return {adapter: undefined, error: "AdapterId not found"};
        }
        return {adapter: undefined, error: undefined};
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
                {
                    /* an adapter's Object Types, or the list of adapters */
                    adapterId ? (
                        adapter ? <ObjectTypes adapterId={adapterId} adapter={adapter} /> : null
                    ) : (
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
                    )
                }
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
