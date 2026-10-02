// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tabs
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState} from "react";

import MuiTabs from "@mui/material/Tabs";
import MuiTab from "@mui/material/Tab";
import Badge from "@mui/material/Badge";
import {useTheme} from "@mui/material/styles";

import {AdaptiveComponent} from "@afw/react";

/*
 * The tab set, by key (or text). Callers often build `tabs` inline, so a new
 * array on every render must not count as new tabs.
 */
const tabsSignature = (tabs) =>
    tabs ? tabs.map((tab, index) => String(tab.key ?? tab.text ?? index)).join("\u0000") : "";

export const Tabs = (props) => {

    const [selectedTab, setSelectedTab] = useState(props.selectedTab || 0);
    const theme = useTheme();

    /* back to props.selectedTab when it changes, or when the tab set does */
    const signature = tabsSignature(props.tabs);
    const [shown, setShown] = useState({ selectedTab: props.selectedTab, signature });
    if (shown.selectedTab !== props.selectedTab || shown.signature !== signature) {
        setShown({ selectedTab: props.selectedTab, signature });
        setSelectedTab(props.selectedTab || 0);
    }

    if (!props.tabs)
        return null;

    /* eslint @typescript-eslint/no-unused-vars: [2, {"args": "after-used", "varsIgnorePattern": "ignore"}] */
    let {style = { height: "100%" }, tabs, gapSpace, onTabSwitch, orientation, selectedTab: ignore, ...rest} = props;

    /* a selection past the end (fewer tabs than before) shows the first tab */
    const current = (selectedTab < tabs.length) ? selectedTab : 0;
    
    return (
        <div style={{ display: "flex", flexDirection: "column", ...style }}>
            <MuiTabs 
                id={props.id}
                data-testid={props["data-testid"]}
                data-component-type={props["data-component-type"]}
                value={current}
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
            <div style={{ flex: 1, minHeight: 0, marginTop: gapSpace, ...(tabs[current]?.style) }}>
                {
                    tabs[current] && (
                        <AdaptiveComponent {...rest} layoutComponent={tabs[current].contains} />
                    )                        
                }    
            </div>
        </div>
    );
};

export default Tabs;
