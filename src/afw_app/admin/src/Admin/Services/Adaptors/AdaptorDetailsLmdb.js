// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Tabs,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

export const AdaptorDetailsLmdb = ({ adaptor }) => {
    
    const theme = useTheme();

    if (!adaptor)
        return null;
    
    const {adaptorId} = adaptor;         
    
    return (
        <div>                            
            <div style={{ padding: theme.spacing(2) }}>
                <Tabs 
                    gapSpace={16}
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
                                                        <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(adaptorId)} text="click here" />
                                                        <span>.</span>
                                                    </div>
                                                }
                                            />
                                        </div>
                                        <Typography size="8" text="Information" />                                            
                                        <ObjectResponsive 
                                            object={adaptor}
                                            editable={false}
                                            filterOptions={{                                                
                                                filterValuedProperties: true,
                                                filterExcludeProperties: [
                                                    "metrics"
                                                ]
                                            }}
                                            embedded={{
                                                component: "Paper"
                                            }}
                                            spacing={theme.spacing(1)}
                                        />
                                    </div>             
                                </div>
                        },
                        {
                            text: "Metrics",
                            contains:
                                <div style={{ paddingTop: theme.spacing(2) }}>
                                    <ObjectResponsive 
                                        object={adaptor}
                                        editable={false}
                                        filterOptions={{                                                
                                            filterValuedProperties: true,
                                            filterProperties: [
                                                "metrics"
                                            ]
                                        }}
                                        embedded={{
                                            component: "Paper"
                                        }}
                                        spacing={theme.spacing(1)}
                                    />      
                                </div>   
                        }
                    ]}
                />
            </div>
        </div>
    ); 
};

export default AdaptorDetailsLmdb;
