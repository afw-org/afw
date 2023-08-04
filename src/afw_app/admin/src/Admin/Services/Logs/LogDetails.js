// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

const LogDetails = ({ log }) => {

    const theme = useTheme();
    if (!log)
        return null;       
    
    const {properties, serviceId} = log;     

    return (
        <div>                              
            <div style={{ paddingTop: theme.spacing(3) }}>                            
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Message
                        contains={
                            <div>
                                <span>To edit this Log's configuration, </span>
                                <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(serviceId)} text="click here" />
                                <span>.</span>
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

export default LogDetails;
