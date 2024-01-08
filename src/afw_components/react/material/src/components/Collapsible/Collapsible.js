// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Collapsible
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState, useEffect} from "react";

import Accordion from "@mui/material/Accordion";
import AccordionSummary from "@mui/material/AccordionSummary";
import AccordionDetails from "@mui/material/AccordionDetails";
import ExpandMore from "@mui/icons-material/ExpandMore";
import {styled} from "@mui/material/styles";

import {AdaptiveComponent} from "@afw/react";

/* 
 * The entire MaterialUI Accordion components seem excessively padded, so I made condensed versions
 * which allows me to override the default styles.
 */
const CompactAccordion = styled((props) => (
    <Accordion {...props} />
))(() => ({    
    padding: "0 12px 4px 12px",
    minHeight: "0px !important",    
    "& .Mui-expanded": {
        margin: "0 0 0 0",
    }
}));

const CompactAccordionSummary = styled((props) => (
    <AccordionSummary {...props} />
))(() => ({    
    minHeight: "0px !important",
    padding: "0 12px 0 0px",    
    "& .Mui-expanded": {},
    "& .MuiAccordionSummary-content": {      
        margin: "0",
    },
    "& .MuiAccordionSummary-content.Mui-expanded": {              
        margin: "0px",        
    }
}));

const CompactAccordionDetails = styled((props) => (
    <AccordionDetails {...props} />
))(() => ({    
    padding: "6px 6px 12px",    
}));


export const Collapsible = (props) => {

    const [isCollapsed, setCollapsed] = useState((props.isCollapsed === false) ? false : true);

    useEffect(() => {
        setCollapsed((props.isCollapsed === false) ? false : true);
    }, [props.isCollapsed]);
   
    const onToggleCollapse = (isCollapsed) => {
        setCollapsed(isCollapsed);

        if (props.onToggleCollapse)
            props.onToggleCollapse(isCollapsed);
    };
       
    const {contains, headerItems} = props; 
    if (!contains)
        return null;             

    return (            
        <CompactAccordion             
            id={props.id}
            data-testid={props["data-testid"]}
            data-component-type={props["data-component-type"]}
            expanded={!isCollapsed}
            onChange={(event, expanded) => onToggleCollapse(!expanded)}
            style={props.style}
        >
            <CompactAccordionSummary 
                id={props.id ? (props.id + "-header") : undefined} 
                aria-controls={props.id ? (props.id + "-content") : undefined}
                aria-label={props["aria-label"]} 
                expandIcon={<ExpandMore />}
            >
                <>                    
                    {
                        props.headerItems &&
                            <AdaptiveComponent layoutComponent={headerItems} />
                    }
                    {
                        props.onRenderHeader &&                            
                            props.onRenderHeader(props)
                    }
                </>
            </CompactAccordionSummary>
            <CompactAccordionDetails>                            
                <div style={{ width: "100%", ...props.bodyStyle }}>
                    {
                        (isCollapsed === true) ? null : 
                            <AdaptiveComponent layoutComponent={contains} />
                    }
                </div>
            </CompactAccordionDetails>
        </CompactAccordion>                
    );
};

export default Collapsible;
