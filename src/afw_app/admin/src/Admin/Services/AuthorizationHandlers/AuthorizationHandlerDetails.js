// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

const AuthorizationHandlerDetails = ({ authHandler }) => {

    const theme = useTheme();
    
    if (!authHandler)
        return null;

    //let authorizationHandlerId = authHandler.getPropertyValue("authorizationHandlerId");
    const {properties, serviceId} = authHandler;
    
    return (
        <div>                             
            <div style={{ paddingTop: theme.spacing(3) }}>
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Message
                        contains={
                            <div>
                                <span>To edit this handler's configuration, </span>
                                <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(serviceId)} text="Click here" />
                            </div>
                        }
                    />
                </div>                    
                <Typography size="8" text="Properties" />                                
                <ObjectResponsive 
                    object={properties}
                    spacing={theme.spacing(1)}
                    filterOptions={{
                        filterValuedProperties: true
                    }}
                />
            </div>
        </div>
    );
};

export default AuthorizationHandlerDetails;
