// See the 'COPYING' file in the project root for licensing information.
import {useParams} from "@tanstack/react-router";

import AuthorizationHandlerDetails from "./AuthorizationHandlerDetails";

import {
    Link,
    Message,
    Table
} from "@afw/react";

import {useAppCore} from "../../../hooks";

const AuthorizationHandlers = () => {

    const {authHandlers} = useAppCore();

    /* the route's optional param (see ../../routes.js) */
    const {authorizationHandlerId: routeAuthorizationHandlerId} = useParams({ strict: false });
    
    /* one authorization handler, chosen by the route's authorizationHandlerId */
    const renderAuthorizationHandler = () => {
        let authHandler;

        authHandlers.forEach((handler) => {
            if (handler.properties.authorizationHandlerId === 
                        routeAuthorizationHandlerId)
                authHandler = handler;
        });

        return (
            <AuthorizationHandlerDetails 
                authHandler={authHandler} 
            />
        );
    };

    return (
        <div>                                
            {
                /* one authorization handler (by the route's authorizationHandlerId), or the list */
                routeAuthorizationHandlerId ? renderAuthorizationHandler() : (
                    <div>                                                   
                        <Message
                            contains={
                                <div>
                                    <span>To create a new Authorization Handler, add a new authorization Type Service </span>
                                    <Link style={{ display: "inline-block" }} url="/Admin/Services/" text="here" />
                                    <span>.</span>
                                </div>
                            }
                        />                                         
                        <div>
                            <Table
                                rows={authHandlers ? authHandlers : []}
                                columns={[
                                    { 
                                        key: "Id", name: "Id", isResizable: true, minWidth: 150, maxWidth: 200,
                                        onRender: (auth) => {
                                            let properties = auth.properties;
                                            let id = properties.authorizationHandlerId;
                                            let url = "/Admin/AuthHandlers/" + encodeURIComponent(id);
            
                                            return (
                                                <Link url={url} text={id} />
                                            );
                                        }
                                    },
                                    {
                                        key: "type", name: "Type", isResizable: true, minWidth: 150, maxWidth: 200,
                                        isMultiline: true,
                                        onRender: (auth) => {
                                            let properties = auth.properties;
                                            let type = properties.authorizationHandlerType;
            
                                            return (
                                                <span>{type}</span>
                                            );
                                        }
                                    },
                                    {
                                        key: "description", name: "Description", isResizable: true, minWidth: 300, maxWidth: 300,
                                        isMultiline: true,
                                        onRender: (auth) => {
                                            let properties = auth.properties;
                                            let description = properties.description;
            
                                            return (
                                                <span>{description}</span>
                                            );
                                        }
                                    }
                                ]}
                                selectionMode="none"
                            />     
                        </div>                       
                    </div>
                )
            }
        </div>
    );
};

export default AuthorizationHandlers;
