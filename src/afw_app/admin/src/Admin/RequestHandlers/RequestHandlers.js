// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import {Switch, Route} from "react-router";

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

    if (!requestHandlers)
        return null;

    return (
        <div id="admin-admin-requestHandlers" data-testid="admin-admin-requestHandlers"  style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={setShowHelp} />                
            </div>
            <div style={{ flex: 1, overflow: "auto" }}>
                <Switch>
                    <Route exact path="/Admin/RequestHandlers/" render={(props) => 
                        <div>                            
                            <Table
                                rows={requestHandlers}
                                columns={[
                                    { 
                                        key: "URI", name: "URI", isResizable: true, minWidth: 150, maxWidth: 200,
                                        onRender: (requestHandler) => {                                                                                                                
                                            const uriPrefix = requestHandler.uriPrefix;
                                            let url = props.match.url + "/" + encodeURIComponent(uriPrefix);
            
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
                    }/>
                    <Route path="/Admin/RequestHandlers/:requestHandlerId" render={(props) => {
                        let requestHandler;

                        requestHandlers.forEach((handler) => {
                            if (handler.uriPrefix === decodeURIComponent(props.match.params.requestHandlerId))
                                requestHandler = handler;
                        });

                        return (
                            <RequestHandlerDetails 
                                {...props} 
                                requestHandler={requestHandler} 
                            />
                        );
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

export default RequestHandlers;
