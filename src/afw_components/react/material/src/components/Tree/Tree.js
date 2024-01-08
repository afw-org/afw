// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tree
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import PropTypes from "prop-types";

import TreeView from "@mui/lab/TreeView";
import TreeItem from "@mui/lab/TreeItem";
import Collapse from "@mui/material/Collapse";
import {alpha} from "@mui/material/styles";
import SvgIcon from "@mui/material/SvgIcon";

import Icon from "../Icon";
import Tooltip from "../Tooltip";
import {AdaptiveComponent, useTheme, useClasses} from "@afw/react";


const MinusSquare = (props) => {
    return (
        <SvgIcon fontSize="inherit" {...props}>
            <path d="M22.047 22.074v0 0-20.147 0h-20.12v0 20.147 0h20.12zM22.047 24h-20.12q-.803 0-1.365-.562t-.562-1.365v-20.147q0-.776.562-1.351t1.365-.575h20.147q.776 0 1.351.575t.575 1.351v20.147q0 .803-.575 1.365t-1.378.562v0zM17.873 11.023h-11.826q-.375 0-.669.281t-.294.682v0q0 .401.294 .682t.669.281h11.826q.375 0 .669-.281t.294-.682v0q0-.401-.294-.682t-.669-.281z" />
        </SvgIcon>
    );
};

const PlusSquare = (props) => {
    return (
        <SvgIcon fontSize="inherit" {...props}>        
            <path d="M22.047 22.074v0 0-20.147 0h-20.12v0 20.147 0h20.12zM22.047 24h-20.12q-.803 0-1.365-.562t-.562-1.365v-20.147q0-.776.562-1.351t1.365-.575h20.147q.776 0 1.351.575t.575 1.351v20.147q0 .803-.575 1.365t-1.378.562v0zM17.873 12.977h-4.923v4.896q0 .401-.281.682t-.682.281v0q-.375 0-.669-.281t-.294-.682v-4.896h-4.923q-.401 0-.682-.294t-.281-.669v0q0-.401.281-.682t.682-.281h4.923v-4.896q0-.401.294-.682t.669-.281v0q.401 0 .682.281t.281.682v4.896h4.923q.401 0 .682.281t.281.682v0q0 .375-.281.669t-.682.294z" />
        </SvgIcon>
    );
};

const CloseSquare = (props) => {
    return (
        <SvgIcon className="close" fontSize="inherit" {...props}>          
            <path d="M17.485 17.512q-.281.281-.682.281t-.696-.268l-4.12-4.147-4.12 4.147q-.294.268-.696.268t-.682-.281-.281-.682.294-.669l4.12-4.147-4.12-4.147q-.294-.268-.294-.669t.281-.682.682-.281.696 .268l4.12 4.147 4.12-4.147q.294-.268.696-.268t.682.281 .281.669-.294.682l-4.12 4.147 4.12 4.147q.294.268 .294.669t-.281.682zM22.047 22.074v0 0-20.147 0h-20.12v0 20.147 0h20.12zM22.047 24h-20.12q-.803 0-1.365-.562t-.562-1.365v-20.147q0-.776.562-1.351t1.365-.575h20.147q.776 0 1.351.575t.575 1.351v20.147q0 .803-.575 1.365t-1.378.562v0z" />
        </SvgIcon>
    );
};

const Styles = theme => ({
    iconContainer: {
        "& .close": {
            opacity: 0.3,
        },
    },
    group: {
        marginLeft: 12,
        paddingLeft: 12,
        borderLeft: `1px dashed ${alpha(theme.palette.text.primary, 0.4)}`,
    },
});

const StyledTreeItem = (props) => {
    const classes = useClasses(Styles);

    return (
        <TreeItem {...props} classes={classes} TransitionComponent={Collapse} />
    );
};

/**
 * Implementation Id : Tree
 * Category          : containers
 * 
 * A component that renders data in a Tree structure.
 * 
 * A component that renders data in a Tree structure.
 * 
 */
export const Tree = (props) => {

    const {
        selectedKeys, 
        expandedKeys, 
        defaultCollapseIcon, 
        defaultExpandIcon,  
        defaultEndIcon,      
    } = props;
    const theme = useTheme();

    const onNodeToggle = (event, expanded) => {
        if (props.onNodeToggle)
            props.onNodeToggle(expanded);
    };

    const onContextMenu = (event, node) => {
        if (props.onContextMenu)
            props.onContextMenu(event, node);
    };

    const renderLabel = (node) => {
        
        let label = (
            // eslint-disable-next-line             
            <div                 
                onClick={event => {
                    if (props.onSelectTreeNode)
                        props.onSelectTreeNode(node);

                    event.stopPropagation();
                    event.preventDefault();
                }}
            >
                {
                    node.label ? node.label : (
                        <AdaptiveComponent layoutComponent={node.contains} />
                    )
                }
            </div>
        );

        if (node.tooltip) {
            label = (
                <Tooltip
                    content={node.tooltip}
                    delay={1000}
                    target={ <div>{ label }</div> }
                />
            );
        }

        return label;
    };

    const renderChild = (node) => (
        <StyledTreeItem 
            key={node.key}
            data-testid={node["data-testid"]}
            nodeId={node.key}
            label={renderLabel(node)}
            icon={node.icon ? <Icon iconName={node.icon} /> : undefined}
            expandIcon={node.expandIcon ? <Icon iconName={node.expandIcon} /> : undefined}
            collapseIcon={node.collapseIcon ? <Icon iconName={node.collapseIcon} /> : undefined}
            onContextMenu={(event) => onContextMenu(event, node)}
        >
            {
                node.children ?
                    node.children.map(renderChild) : null
            }
        </StyledTreeItem>
    );

    return (
        <TreeView 
            id={props.id}
            aria-label={props["aria-label"]}
            data-testid={props["data-testid"]}
            data-component-type={props["data-component-type"]}
            className={props.className}
            style={props.style}
            defaultCollapseIcon={defaultCollapseIcon || <MinusSquare style={{ color: theme.palette.primary.main }} />}
            defaultExpandIcon={defaultExpandIcon || <PlusSquare style={{ color: theme.palette.primary.main }} />}
            defaultEndIcon={defaultEndIcon || <CloseSquare style={{ color: theme.palette.primary.main }} />}  
            onNodeToggle={onNodeToggle} 
            onNodeSelect={() => undefined}
            expanded={expandedKeys}
            selected={selectedKeys}
        >
            {
                props.children ?
                    props.children.map(renderChild) : null
            }    
        </TreeView>
    );

};

/* Generated objectType propType definition for property label */
let Tree_Node;
Tree_Node = {
    /**
     * Property: children
     * Data Type: (list, object _AdaptiveLayoutComponentType_Tree_Node)
     * 
     * The children of this Tree Node.
     */
    children:                           PropTypes.arrayOf(PropTypes.shape(Tree_Node)),
    /**
     * Property: contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Optional component under this tree node.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * Property: icon
     * Data Type: (string)
     * 
     * The icon for this tree node.
     */
    icon:                               PropTypes.string,
    /**
     * Property: key
     * Data Type: (string)
     * 
     * The unique identifying key for this tree node.
     */
    key:                                PropTypes.string.isRequired,
    /**
     * Property: label
     * Data Type: (string)
     * 
     * The label for this tree node.
     */
    label:                              PropTypes.string,
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Tree
 */
Tree.propTypes = {
    /**
     * children
     * Data Type: (list, object _AdaptiveLayoutComponentType_Tree_Node)
     * 
     * The children of this Tree.
     */
    children:                           PropTypes.arrayOf(PropTypes.shape(Tree_Node)).isRequired,
    /**
     * dense
     * Data Type: (boolean)
     * 
     * The tree nodes should be condensed in space.
     */
    dense:                              PropTypes.bool,
    /**
     * expandedKeys
     * Data Type: (list, string)
     * 
     * The keys of nodes in this tree that are expanded.
     */
    expandedKeys:                       PropTypes.arrayOf(PropTypes.string),
    /**
     * selectedKeys
     * Data Type: (list, string)
     * 
     * The keys of nodes in this tree that are selected.
     */
    selectedKeys:                       PropTypes.arrayOf(PropTypes.string),
};

Tree.defaultProps = {
};

export default Tree;
