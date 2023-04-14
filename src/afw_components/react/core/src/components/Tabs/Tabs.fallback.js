// See the 'COPYING' file in the project root for licensing information.
export const Tabs = ({ tabs, gapSpace, selectedTab, onTabSwitch }) => {    
    return (
        <div>
            <div 
                style={{ 
                    display: "flex", 
                    marginBottom: gapSpace,                    
                }}
            >
                {
                    tabs && tabs.map((tab, tabIndex) =>
                        <div 
                            key={tabIndex}
                            style={{ 
                                marginRight: "16px",
                                borderBottom: (tab.text == selectedTab.text) ? "solid #999 thin" : undefined,                                
                            }}
                            role="tab"
                            tabIndex="-1"
                            onClick={() => {                                
                                if (onTabSwitch)
                                    onTabSwitch(tabIndex, tab);
                            }}
                            onKeyDown={() => {
                                if (onTabSwitch)
                                    onTabSwitch(tabIndex, tab);
                            }}                            
                        >
                            <span>{ tab.text }</span>
                        </div>
                    )
                }
            </div>
            {
                selectedTab && tabs && (
                    selectedTab.contains
                )
            }
        </div>
    );
};

export default Tabs;
