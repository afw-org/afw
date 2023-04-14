// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Tabs,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

export const AdaptorDetails = ({ adaptor }) => {

    const theme = useTheme();
    
    if (!adaptor)
        return null;
           
    const {serviceId} = adaptor;

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
                                                        <span>To edit this adaptor's configuration, </span>
                                                        <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(serviceId)} text="click here" />
                                                        <span>.</span>
                                                    </div>
                                                }
                                            />
                                        </div>
                                        <Typography size="8" text="Properties" />
                                        <ObjectResponsive 
                                            object={adaptor}
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

export default AdaptorDetails;
