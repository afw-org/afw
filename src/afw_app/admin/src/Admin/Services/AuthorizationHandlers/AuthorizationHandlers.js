// See the 'COPYING' file in the project root for licensing information.
import {Switch, Route, useRouteMatch} from "react-router";

import AuthorizationHandlerDetails from "./AuthorizationHandlerDetails";

import {
    Link,
    Message,
    Table
} from "@afw/react";

import {useAppCore} from "../../../hooks";

const AuthorizationHandlers = () => {

    const {authHandlers} = useAppCore();

    const match = useRouteMatch();
    
    return (
        <div>                                
            <Switch>
                <Route exact path="/Admin/AuthHandlers/" render={() => 
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
                                            let url = match.url + "/" + encodeURIComponent(id);
            
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
                }/>
                <Route path="/Admin/AuthHandlers/:authorizationHandlerId" render={(props) => {
                    let authHandler;

                    authHandlers.forEach((handler) => {
                        if (handler.properties.authorizationHandlerId === 
                                decodeURIComponent(props.match.params.authorizationHandlerId))
                            authHandler = handler;
                    });

                    return (
                        <AuthorizationHandlerDetails 
                            {...props} 
                            authHandler={authHandler} 
                        />
                    );
                }} />
            </Switch>
        </div>
    );
};

export default AuthorizationHandlers;
