// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tabs
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState, useEffect} from "react";

import MuiTabs from "@mui/material/Tabs";
import MuiTab from "@mui/material/Tab";
import Badge from "@mui/material/Badge";
import {useTheme} from "@mui/material/styles";

import {AdaptiveComponent} from "@afw/react";

export const Tabs = (props) => {

    const [selectedTab, setSelectedTab] = useState(props.selectedTab || 0);
    const theme = useTheme();

    useEffect(() => {
        setSelectedTab(props.selectedTab || 0);
    }, [props.selectedTab, props.tabs]);

    if (!props.tabs)
        return null;

    // eslint-disable-next-line no-unused-vars
    let {style = { height: "100%" }, tabs, gapSpace, onTabSwitch, orientation, selectedTab: ignore, ...rest} = props;
    
    return (
        <div style={{ display: "flex", flexDirection: "column", ...style }}>
            <MuiTabs 
                id={props.id}
                data-testid={props["data-testid"]}
                data-component-type={props["data-component-type"]}
                value={selectedTab}
                onChange={(event, selectedTab) => {
                    setSelectedTab( selectedTab );
                    if (onTabSwitch)
                        onTabSwitch(tabs[selectedTab], selectedTab);
                }}
                variant="scrollable"
                scrollButtons="auto"        
                textColor="primary"
                orientation={orientation}
                aria-label={props["aria-label"]}                    
            >
                {
                    tabs.map((tab, index) => 
                        <MuiTab
                            key={tab.key || index} 
                            label={
                                tab.badge ? 
                                    <Badge color="primary" style={{ marginRight: theme.spacing(4) }} 
                                        badgeContent={tab.badge}>{tab.text}
                                    </Badge> : 
                                    tab.text
                            }       
                            aria-label={tab.ariaLabel || tab.text}                                  
                        />)
                }
            </MuiTabs>                    
            <div style={{ flex: 1, marginTop: gapSpace, ...(tabs[selectedTab]?.style) }}>
                {
                    tabs[selectedTab] && (
                        <AdaptiveComponent {...rest} layoutComponent={tabs[selectedTab].contains} />
                    )                        
                }    
            </div>
        </div>
    );
};

export default Tabs;
