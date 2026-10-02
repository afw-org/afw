// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import {useParams} from "@tanstack/react-router";

import RequestHandlerDetails from "./RequestHandlerDetails";
import {useAppCore, useBreadcrumbs, useTheme} from "../../hooks";

import {
    Breadcrumb,
    Link,
    Table
} from "@afw/react";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";

const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

const RequestHandlers = () => { 
    
    const theme = useTheme();
    const [showHelp, setShowHelp] = useState(false);
    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);

    const {requestHandlers} = useAppCore();
    /* the route's optional param (see ../routes.js) */
    const {requestHandlerId: routeRequestHandlerId} = useParams({ strict: false });

    if (!requestHandlers)
        return null;

    /* one request handler, chosen by the route's requestHandlerId */
    const renderRequestHandler = () => {
        let requestHandler;

        requestHandlers.forEach((handler) => {
            if (handler.uriPrefix === routeRequestHandlerId)
                requestHandler = handler;
        });

        return (
            <RequestHandlerDetails 
                requestHandler={requestHandler} 
            />
        );
    };

    return (
        <div id="admin-admin-requestHandlers" data-testid="admin-admin-requestHandlers"  style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={setShowHelp} />                
            </div>
            <div style={{ flex: 1, overflow: "auto" }}>
                {
                    /* one request handler (by the route's requestHandlerId), or the list */
                    routeRequestHandlerId ? renderRequestHandler() : (
                        <div>                            
                            <Table
                                rows={requestHandlers}
                                columns={[
                                    { 
                                        key: "URI", name: "URI", isResizable: true, minWidth: 150, maxWidth: 200,
                                        onRender: (requestHandler) => {                                                                                                                
                                            const uriPrefix = requestHandler.uriPrefix;
                                            let url = "/Admin/RequestHandlers/" + encodeURIComponent(uriPrefix);
            
                                            return (
                                                <Link url={url} text={requestHandler.uriPrefix} />
                                            );
                                        }
                                    },
                                    {
                                        key: "Type", name: "Type", isResizable: true, minWidth: 150, maxWidth: 200,
                                        onRender: (requestHandler) => {
                                            const handlerType = requestHandler.requestHandlerType;

                                            return <span>{handlerType}</span>;
                                        }
                                    },
                                    {
                                        key: "description", name: "Description", isResizable: true, minWidth: 300, maxWidth: 300,
                                        isMultiline: true,
                                        onRender: (requestHandler) => {                                            
                                            const description = requestHandler.description;
            
                                            return (
                                                <span>{description}</span>
                                            );
                                        }
                                    }
                                ]}
                                selectionMode="none"
                            />                            
                        </div>
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

export default RequestHandlers;
