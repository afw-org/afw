// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Tabs,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

export const AdapterDetails = ({ adapter }) => {

    const theme = useTheme();
    
    if (!adapter)
        return null;
           
    const {serviceId} = adapter;

    return (
        <div>                
            <div style={{ padding: theme.spacing(2) }}>
                <Tabs 
                    gapSpace={20}
                    tabs={[
                        {
                            text: "General",
                            contains: 
                                <div>
                                    <div style={{ paddingTop: theme.spacing(3) }}>                            
                                        <div style={{ paddingBottom: theme.spacing(2) }}>
                                            <Message 
                                                contains={
                                                    <div>
                                                        <span>To edit this adapter's configuration, </span>
                                                        <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(serviceId)} text="click here" />
                                                        <span>.</span>
                                                    </div>
                                                }
                                            />
                                        </div>
                                        <Typography size="8" text="Properties" />
                                        <ObjectResponsive 
                                            object={adapter}
                                            editable={false}
                                            filterOptions={{                                                
                                                filterValuedProperties: true,
                                            }}
                                            embedded={{
                                                component: "None"
                                            }}
                                            spacing={theme.spacing(1)}
                                        />                                        
                                    </div> 
                                </div>
                        },
                        {
                            text: "Additional Metrics",
                            contains: 
                                <div />
                        }
                    ]} 
                />    
            </div>      
        </div>
    );  
};

export default AdapterDetails;
