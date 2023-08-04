// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";
import PropTypes from "prop-types";

import {
    Button,
    Divider,
    Tree
} from "@afw/react";

import {PropertyResponsive} from "../ObjectResponsive";

//import {useOperational} from "@afw/react";
import {useAdaptiveLayout, useTheme} from "@afw/react";
import {AfwObject} from "@afw/client";


const buildProperty = (property, prefix, showLabels) => {
    if (property.getDataType() === "object")
        // eslint-disable-next-line @typescript-eslint/no-use-before-define
        return buildTree(property.getValue(), prefix, showLabels);
    else if (property.getDataType() === "list") { 
        // eslint-disable-next-line @typescript-eslint/no-use-before-define
        return buildList(property, prefix, showLabels);
    } else {
        return null;            
    } 
};

const buildList = (node, prefix, showLabels) => {
    let treeChildren = [];

    if (node) {
        if (node.getValue()) {
            node.getValue().forEach((value, index) => {
                treeChildren.push({
                    key: prefix + node.getName() + "_" + String(index),
                    label: "[" + String(index) + "]",
                    children: buildProperty(value, prefix + String(index) + "_", showLabels),
                    property: value,
                });
            });
        }
    }

    return treeChildren;
};

const buildTree = (node, prefix = "", showLabels) => {   
    let treeChildren = [];

    if (node) {            
        /* when object changes, trigger refresh */            
        let properties = node.getProperties();
        properties.forEach(property => {
            if (property.getValue() !== undefined) {
                const label = property.getLabel() ? property.getLabel() : property.getName();
                treeChildren.push({
                    key: prefix + property.getName(),
                    label: (showLabels === false) ? property.getName() : label,
                    property,            
                    children: buildProperty(property, prefix + property.getName() + "_", showLabels),                 
                    tooltip: property.getBrief(),       
                });
            }
        });
    }

    return treeChildren;
};

export const ObjectTree = (props) => {

    const [selectedTreeNode, setSelectedTreeNode] = useState();
    const { data: { object }, layoutParameters } = useAdaptiveLayout(props);

    const theme = useTheme();
    const {showLabels} = layoutParameters;        

    const treeChildren = useMemo(() => buildTree(object, "", showLabels), [object, showLabels]);

    const onSelectTreeNode = (node) => {
        setSelectedTreeNode(node);
    };
    
    const {splitView} = props;
    //const {editable} = useOperational(props);

    if (splitView) {
        return (     
            <div style={{ height: "100%" }}>    
                <div style={{ maxHeight: (selectedTreeNode ? "60%" : undefined), overflow: "auto", padding: "4px" }}> 
                    <Tree 
                        children={treeChildren}   
                        onSelectTreeNode={onSelectTreeNode}
                        selectionMode="single"
                    />
                </div>
                {
                    selectedTreeNode &&             
                        <>
                            <Divider />
                            <div style={{ display: "flex" }}>                                
                                <div style={{ flex: 1 }}> 
                                    <div style={{ height: theme.spacing(2) }} />
                                    <PropertyResponsive
                                        property={selectedTreeNode.property}
                                        filterOptions={{
                                            filterValuedProperties: true
                                        }}
                                    />
                                </div>   
                                <Button 
                                    type="icon" 
                                    label="Close"
                                    tooltip="Close"
                                    icon="close" 
                                    onClick={() => setSelectedTreeNode(undefined)} 
                                />    
                            </div>     
                        </>       
                }        
            </div>
        );
    } else {
        return (            
            <Tree 
                children={treeChildren}   
                onSelectTreeNode={onSelectTreeNode}             
                selectionMode="single"
            />
        );
    }
};

ObjectTree.propTypes = {
    object:                 PropTypes.instanceOf(AfwObject).isRequired,
    splitView:              PropTypes.bool,
    editable:               PropTypes.bool,
};

ObjectTree.defaultProps = {
    splitView:              true,
};

export default ObjectTree;
