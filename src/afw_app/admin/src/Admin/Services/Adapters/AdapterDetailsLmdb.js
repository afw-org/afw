// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Message,
    ObjectResponsive,
    Tabs,
    Typography
} from "@afw/react";

import {useTheme} from "../../../hooks";

export const AdapterDetailsLmdb = ({ adapter }) => {
    
    const theme = useTheme();

    if (!adapter)
        return null;
    
    const {adapterId} = adapter;         
    
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
                                                        <span>To edit this adapter's configuration, </span>
                                                        <Link style={{ display: "inline-block" }} url={"/Admin/Services/" + encodeURIComponent(adapterId)} text="click here" />
                                                        <span>.</span>
                                                    </div>
                                                }
                                            />
                                        </div>
                                        <Typography size="8" text="Information" />                                            
                                        <ObjectResponsive 
                                            object={adapter}
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
                                        object={adapter}
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

export default AdapterDetailsLmdb;
