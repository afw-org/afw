// See the 'COPYING' file in the project root for licensing information.
import {useReducer, useEffect} from "react";
import {useLocation, useHistory} from "react-router";

import {Tree, useOperational} from "@afw/react";
import {debounce} from "@afw/client";

import {ModelTreeContextMenu} from "./ModelTreeContextMenu";
import {ModelTreeView} from "./ModelTreeView";

import {useTheme} from "../../../hooks";
import SplitPane from "../../../common/SplitPane";



/**
 * ModelTree
 * 
 * These components represent the Model in a TreeView way, where you can navigate the 
 * hierarchy of a model using a Tree on the left-hand side and view/edit content in 
 * the window on the right-hand side.
 * 
 */

/* buildUri() joins uri parts and encodes them individually, and prepends/appends the appropriate strings */
const buildUri = (parts) => {
    let uri = "/Admin/Models/";

    uri += parts.map(p => encodeURI(p)).join("/");

    return uri + "#tree";
};


/**
 * buildCustomTree()
 * 
 * Builds a Tree structure for Custom Variables.
 */
const buildCustomTree = (model, treeIndex) => {

    const customTree = [];

    const {custom, modelId} = model.getPropertyValues();
    const adapterId = model.getAdapterId();

    if (custom) {
        if (custom.getProperties()) {
            const sorted = custom.getProperties().sort((A, B) => A.getName().localeCompare(B.getName()));
            sorted.forEach(c => {
                const name = c.getName();

                customTree.push({
                    key: modelId + ".custom." + name,  
                    "data-testid": modelId + ".custom." + name,
                    type: "custom",
                    label: name,
                    uri: buildUri([adapterId, modelId, "custom", name]),
                    propertyName: ["custom", name],
                    icon: "Variable",
                });

                treeIndex[modelId + ".custom." + name] = customTree[customTree.length -1];
            });
        }
    }

    return customTree;
};

/**
 * 
 * buildObjectTypeTree()
 * 
 * Builds a Tree structure for an Object Type.
 */
const buildObjectTypeTree = (model, treeIndex, objectType) => {

    const adapterId = model.getAdapterId();
    const modelId = model.getPropertyValue("modelId");
    const name = objectType.getName();
    const keyPrefix = modelId + ".objectTypes." + name;    
    const uriPrefix = [ adapterId, modelId, "objectTypes", name ];

    const node = {
        key: keyPrefix,
        "data-testid": keyPrefix,
        type: "objectType",
        label: name,
        propertyName: ["objectTypes", name],
        objectType: name,
        uri: buildUri([ ...uriPrefix ]),
        children: [
            {
                key: keyPrefix + ".onFunctions",
                "data-testid": keyPrefix + ".onFunctions",
                type: "onFunctions",
                label: "On Functions",
                objectType: name,
                uri: buildUri([ ...uriPrefix, "onFunctions" ]),
                children: [
                    {
                        key: keyPrefix + ".onFunctions.onGetInitialObjectId",
                        "data-testid": keyPrefix + ".onFunctions.onGetInitialObjectId",
                        type: "onFunction",                                    
                        label: "On Get Initial ObjectId()",
                        propertyName: ["objectTypes", name, "onGetInitialObjectId"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onGetInitialObjectId" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onAddObject",
                        "data-testid": keyPrefix + ".onFunctions.onAddObject",
                        type: "onFunction",
                        label: "On Add Object()",
                        propertyName: ["objectTypes", name, "onAddObject"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onAddObject" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onModifyObject",
                        "data-testid": keyPrefix + ".onFunctions.onModifyObject",
                        type: "onFunction",
                        label: "On Modify Object()",
                        propertyName: ["objectTypes", name, "onModifyObject"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onModifyObject" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onReplaceObject",
                        "data-testid": keyPrefix + ".onFunctions.onReplaceObject",
                        type: "onFunction",
                        label: "On Replace Object()",
                        propertyName: ["objectTypes", name, "onReplaceObject"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onReplaceObject" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onDeleteObject",
                        "data-testid": keyPrefix + ".onFunctions.onDeleteObject",
                        type: "onFunction",
                        label: "On Delete Object()",
                        propertyName: ["objectTypes", name, "onDeleteObject"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onDeleteObject" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onGetObject",
                        "data-testid": keyPrefix + ".onFunctions.onGetObject",
                        type: "onFunction",
                        label: "On Get Object()",
                        propertyName: ["objectTypes", name, "onGetObject"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onGetObject" ]),
                    },
                    {
                        key: keyPrefix + ".onFunctions.onRetrieveObjects",
                        "data-testid": keyPrefix + ".onFunctions.onRetrieveObjects",
                        type: "onFunction",
                        label: "On Retrieve Objects()",
                        propertyName: ["objectTypes", name, "onRetrieveObjects"],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onRetrieveObjects" ]),
                    },
                ]
            },
            {
                key: keyPrefix + ".custom",
                "data-testid": keyPrefix + ".custom",
                type: "customs",
                label: "Custom Variables",
                objectType: name,
                propertyName: ["objectTypes", name, "custom"],
                uri: buildUri([ ...uriPrefix, "custom" ]),
                children: []
            },
            {
                key: keyPrefix + ".propertyTypes",
                "data-testid": keyPrefix + ".propertyTypes",
                type: "propertyTypes",
                label: "Property Types",
                objectType: name,
                propertyName: ["objectTypes", name, "propertyTypes"],
                uri: buildUri([ ...uriPrefix, "propertyTypes" ]),
                children: []
            },
            {
                key: keyPrefix + ".methods",
                "data-testid": keyPrefix + ".methods",
                type: "methods",
                label: "Methods",
                objectType: name,
                uri: buildUri([ ...uriPrefix, "methods" ]),
                children: [],
            }
        ],
    };

    /* iterate through all custom variables on this objectType */
    const custom = objectType.getValue()?.getPropertyValue("custom");
    if (custom) {
        if (custom.getProperties()) {
            const sorted = custom.getProperties().sort((A, B) => A.getName().localeCompare(B.getName()));
            sorted.forEach(c => {
                node.children[1].children.push({
                    key: keyPrefix + ".custom." + c.getName(),
                    "data-testid": keyPrefix + ".custom." + c.getName(),
                    type: "custom",
                    label: c.getName(),
                    propertyName: [ "objectTypes", name, "custom", c.getName()],
                    uri: buildUri([ ...uriPrefix, "custom", c.getName() ]),
                    icon: "Variable"
                });

                treeIndex[ keyPrefix + ".custom." + c.getName()] = node.children[1].children[node.children[1].children.length - 1];
            });
        }
    }

    /* index the tree */
    treeIndex[keyPrefix] = node;
    node.children.forEach(c => treeIndex[c.key] = c);                
    node.children[0].children.forEach(c => treeIndex[c.key] = c);    

    return node;
};

/**
 * 
 * buildPropertyTypeTree()
 * 
 * Builds a Tree structure for Property Types.
 */
const buildPropertyTypeTree = (model, treeIndex, objectType, propertyType) => {

    const adapterId = model.getAdapterId();
    const modelId = model.getPropertyValue("modelId");
    const keyPrefix = modelId + ".objectTypes." + objectType.getName() + ".propertyTypes." + propertyType.getName();
    const uriPrefix = [ adapterId, modelId, "objectTypes", objectType.getName(), "propertyTypes", propertyType.getName() ];
    const propertyNamePrefix = [ "objectTypes", objectType.getName(), "propertyTypes", propertyType.getName() ];

    const node = {
        key: keyPrefix,
        "data-testid": keyPrefix,
        type: "propertyType",
        label: propertyType.getName(),
        propertyName: [ ...propertyNamePrefix ],
        objectType: objectType.getName(),
        propertyType: propertyType.getName(),
        uri: buildUri([ ...uriPrefix ]),                                    
        children: [
            { 
                key: keyPrefix +  ".onFunctions",
                "data-testid": keyPrefix +  ".onFunctions",
                label: "On Functions",
                propertyType: propertyType.getName(),
                uri: buildUri([ ...uriPrefix, "onFunctions" ]),
                children: [
                    {
                        key: keyPrefix +  ".onFunctions.onGetInitialValue",
                        "data-testid": keyPrefix +  ".onFunctions.onGetInitialValue",
                        type: "onFunction",
                        label: "On Get Initial Value()",
                        propertyName: [ ...propertyNamePrefix, "onGetInitialValue" ],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onGetInitialValue" ]),
                    },
                    {
                        key: keyPrefix +  ".onFunctions.onGetProperty",
                        "data-testid": keyPrefix +  ".onFunctions.onGetProperty",
                        type: "onFunction",
                        label: "On Get Property()",
                        propertyName: [ ...propertyNamePrefix, "onGetProperty" ],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onGetProperty" ]),
                    },
                    {
                        key: keyPrefix +  ".onFunctions.onSetProperty",
                        "data-testid": keyPrefix +  ".onFunctions.onSetProperty",
                        type: "onFunction",
                        label: "On Set Property()",
                        propertyName: [ ...propertyNamePrefix, "onSetProperty" ],
                        icon: "FunctionVariant",
                        uri: buildUri([ ...uriPrefix, "onFunctions", "onSetProperty" ]),
                    },
                ]
            },
            {
                key: keyPrefix +  ".custom",
                "data-testid": keyPrefix +  ".custom",
                type: "customs",
                label: "Custom Variables",
                propertyName: [ ...propertyNamePrefix, "custom" ],
                uri: buildUri([ ...uriPrefix, "custom" ]),
                children: []
            }
        ],
    };

    treeIndex[keyPrefix] = node;
    node.children.forEach(c => treeIndex[c.key] = c);                
    node.children[0].children.forEach(c => treeIndex[c.key] = c);    

    if (propertyType.getValue()?.getPropertyValue("custom")) {
        const propertyCustom = propertyType.getValue().getPropertyValue("custom").getProperties().sort(
            (A, B) => A.getName().localeCompare(B.getName())
        );
        propertyCustom.forEach(pc => {
            const newCustomNode = {
                key: keyPrefix + ".custom." + pc.getName(),
                "data-testid": keyPrefix + ".custom." + pc.getName(),
                type: "custom",
                label: pc.getName(),
                propertyName: [ ...propertyNamePrefix, "custom", pc.getName() ],
                uri: buildUri([ ...uriPrefix, "custom", pc.getName() ]),
                icon: "Variable"
            };

            node.children[1].children.push(newCustomNode);
            treeIndex[keyPrefix + ".custom." + pc.getName()] = newCustomNode;
        });
    }

    return node;
};

/**
 * buildObjectTypesTree()
 * 
 * Builds a Tree structure for Object Types.  It includes each Object Type, along with
 * its Custom Variables, Property Types and Actions.
 */
const buildObjectTypesTree = (model, treeIndex) => {

    const objectTypesTree = [];
    const modelId = model.getPropertyValue("modelId");
    
    const objectTypes = model.getPropertyValue("objectTypes");
    if (objectTypes) {
        if (objectTypes.getProperties()) {
            /* iterate through all of the object types */            
            const sorted = objectTypes.getProperties().sort((A, B) => A.getName().localeCompare(B.getName()));
            sorted.forEach(o => {
                // !\fixme apply style/label/icon to inherited properties                   
                //const inheritedFrom = o.getInheritedFrom();
                const node = buildObjectTypeTree(model, treeIndex, o);                           
                
                /* iterate through all of the propertyTypes on this objectType */
                const propertyTypes = o.getValue()?.getPropertyValue("propertyTypes");
                if (propertyTypes) {
                    if (propertyTypes.getProperties()) {
                        const sorted = propertyTypes.getProperties().sort((A, B) => { 
                            return A.getName().localeCompare(B.getName()); 
                        });
                        sorted.forEach(p => {         

                            // !\fixme apply style/label/icon to inherited properties                   
                            //const inheritedFrom = p.getInheritedFrom();
                            if (p.getValue() && p.getValue().getPropertyValue("method")) {
                                node.children[3].children.push({
                                    key: modelId + ".objectTypes." + o.getName() + ".propertyTypes." + p.getName(),
                                    "data-testid": modelId + ".objectTypes." + o.getName() + ".propertyTypes." + p.getName(),
                                    type: "method",
                                    propertyName: [ "objectTypes", o.getName(), "propertyTypes", p.getName() ],
                                    objectType: o.getName(),
                                    propertyType: p.getName(),
                                });
                            } else {
                                const newNode = buildPropertyTypeTree(model, treeIndex, o, p);                                
                                node.children[2].children.push(newNode);                                
                            }
                            
                        });
                    }
                }

                objectTypesTree.push(node);
            });
        }
    }

    return objectTypesTree;
};


/**
 * buildPropertyTypesTree()
 * 
 * Builds a tree structure for Property Types.
 */
const buildPropertyTypesTree = (model, treeIndex) => {

    const adapterId = model.getAdapterId();
    const modelId = model.getPropertyValue("modelId");
    const propertyTypesTree = [];    

    const propertyTypes = model.getPropertyValue("propertyTypes");
    if (propertyTypes) {
        if (propertyTypes.getProperties()) {
            propertyTypes.getProperties().forEach(p => {
                const keyPrefix = modelId + ".propertyTypes." + p.getName();
                const uriPrefix = [ adapterId, modelId, "propertyTypes", p.getName() ];

                const newNode = {
                    key: keyPrefix,
                    "data-testid": keyPrefix,
                    type: modelId + ".propertyType",
                    label: p.getName(),
                    propertyName: ["propertyTypes", p.getName()],
                    children: [
                        {
                            key: keyPrefix +  ".custom",
                            "data-testid": keyPrefix + ".custom",
                            type: "customs",
                            label: "Custom Variables",
                            uri: buildUri([ ...uriPrefix, "custom" ]),
                            propertyName: [ "propertyTypes", p.getName(), "custom" ],
                            children: []
                        }
                    ],
                    uri: buildUri([ adapterId, modelId, "propertyTypes", p.getName() ]),                                      
                };

                treeIndex[keyPrefix] = propertyTypesTree[propertyTypesTree.length - 1];

                if (p.getValue().getPropertyValue("custom")) {
                    const propertyCustom = p.getValue().getPropertyValue("custom").getProperties().sort(
                        (A, B) => A.getName().localeCompare(B.getName())
                    );
                    propertyCustom.forEach(pc => {
                        const newCustomNode = {
                            key: keyPrefix + ".custom." + pc.getName(),
                            "data-testid": keyPrefix + ".custom." + pc.getName(),
                            type: "custom",
                            label: pc.getName(),
                            propertyName: [ "propertyTypes", p.getName(), "custom", pc.getName() ],
                            uri: buildUri([ ...uriPrefix, "custom", pc.getName() ]),
                            icon: "Variable"
                        };

                        newNode.children[0].children.push(newCustomNode);
                        treeIndex[keyPrefix + ".custom." + pc.getName()] = newCustomNode;
                    });
                }

                propertyTypesTree.push(newNode);
            });
        }
    }

    return propertyTypesTree;
};

const initialState = {
    modelTree:          [],
    treeIndex:          {},
    tree:               [],
    defaultPanelSize:   350,
    expandedKeys:       [],
    selectedKeys:       [],    
};

const reducer = (state, action) => {

    switch (action.type) {

    case "INIT":  
    case "RELOAD_TREE":      

        return {
            ...state,
            tree: action.tree,
            modelTree: action.modelTree,
            treeIndex: action.treeIndex,
        };
    
    case "URL_CHANGE":
        return {
            ...state,
            selectedKeys: [ action.fullKey ],
            expandedKeys: [ ...state.expandedKeys, ...action.expanded ],
            selectedNode: state.treeIndex[action.key],
        };

    case "SELECT_TREE_NODE":
        return {
            ...state,
            selectedNode: action.node,
            selectedKeys: [ action.node.key ],
        };

    case "TOGGLE_TREE_NODE":
        return {
            ...state,
            expandedKeys: action.node
        };

    case "ON_CONTEXT_MENU":
        return {
            ...state,
            contextMenuNode: action.node,
            contextMenuClientX: action.event.clientX,
            contextMenuClientY: action.event.clientY,
        };

    case "CLOSE_CONTEXT_MENU":
        return {
            ...state,
            contextMenuNode: undefined,
            contextMenuClientX: undefined,
            contextMenuClientY: undefined,
        };

    case "RESIZE":
        return {
            ...state,
            defaultPanelSize: action.size,
        };

    default:
        return state;
    }
};


/**
 * ModelTree
 * 
 * This component renders a split view of the Model, with a Tree on one side 
 * which the user can navigate by expanding and selecting nodes, and the appropriate
 * expressions can be edited on the other side.
 */
export const ModelTree = ({ model, reload }) => {

    const [state, dispatch] = useReducer(reducer, initialState);
    
    const {pathname} = useLocation();
    const history = useHistory();
    const theme = useTheme();
    const {editable} = useOperational();

    const {
        modelTree,
        tree,
        selectedNode,
        expandedKeys,
        selectedKeys,
        contextMenuNode,
        contextMenuClientX,
        contextMenuClientY,
        defaultPanelSize,
    } = state;

    const adapterId = model.getAdapterId();
    const modelId = model.getPropertyValue("modelId");

    /*
     * When the model is ready, parse it out to build our Menu Tree.
     */
    useEffect(() => {
        if (model) {
            const modelTree = [];
            const treeIndex = {};

            const customTree = buildCustomTree(model, treeIndex);
            modelTree.push({
                key: modelId + ".custom",
                "data-testid": modelId + ".custom",
                type: "customs",
                label: "Custom Variables",
                propertyName: ["custom"],
                children: customTree,
                uri: buildUri([ adapterId, modelId, "custom" ]),
            });
            treeIndex[modelId + ".custom"] = modelTree[0];

            const objectTypesTree = buildObjectTypesTree(model, treeIndex);
            modelTree.push({
                key: modelId + ".objectTypes",
                "data-testid": modelId + ".objectTypes",
                type: "objectTypes",
                label: "Object Types",
                propertyName: ["objectTypes"],
                children: objectTypesTree,
                uri: buildUri([ adapterId, modelId, "objectTypes" ]),
            });
            treeIndex[modelId + ".objectTypes"] = modelTree[1];

            const propertyTypesTree = buildPropertyTypesTree(model, treeIndex);
            modelTree.push({
                key: modelId + ".propertyTypes",
                "data-testid": modelId + ".propertyTypes",
                type: "propertyTypes",
                label: "Property Types",
                propertyName: ["propertyTypes"],
                children: propertyTypesTree,
                uri: buildUri([ adapterId, modelId, "propertyTypes" ]),                
            });
            treeIndex[modelId + ".propertyTypes"] = modelTree[2];         
            
            const tree = {
                key: modelId,
                "data-testid": modelId,
                type: "model",
                label: modelId,
                children: modelTree,
                uri: buildUri([ adapterId, modelId ]),
            };
            treeIndex[modelId] = tree;

            dispatch({ type: "INIT", tree: [ tree ], modelTree, treeIndex });            
        }
    }, [model, adapterId, modelId, reload]);

    /*
     * When the location url changes, set the expanded nodes, and selected node, as appropriate.
     */
    useEffect(() => {        
        if (model && modelId && modelTree && modelTree.length > 0) {
            let expanded = [];
            let selectedNode;

            /* always expand the top-level tree item (the model) */
            expanded.push( modelId );

            /* select the node that is requested in path */
            const key = pathname.split("/").slice(4).join(".");            

            /* use the path to determine which nodes to expand and select */     
            let parts = pathname.split("/")[4];
            expanded.push( parts );
            
            if (pathname.split("/").length > 4) {
                for (let part of pathname.split("/").slice(5)) {
                    parts += "." + part;
                    expanded.push( parts );
                }
            }            
            
            /* slice off the /Admin/models/:modelId and turn it into a key */
            const fullKey = pathname.split("/").slice(4).join(".");

            dispatch({ type: "URL_CHANGE", selectedNode, expanded, key, fullKey });
        }

    }, [pathname, model, modelId, modelTree]);


    /* When properties are added/removed/renamed, we need to rebuild the tree and its indexes */
    const onReloadTree = () => {
        const modelTree = [];
        const treeIndex = {};

        const customTree = buildCustomTree(model, treeIndex);
        modelTree.push({
            key: modelId + ".custom",
            "data-testid": modelId + ".custom",
            type: "customs",
            label: "Custom Variables",
            propertyName: ["custom"],
            children: customTree,
            uri: buildUri([adapterId, modelId, "custom"]),
        });
        treeIndex[modelId + ".custom"] = modelTree[0];

        const objectTypesTree = buildObjectTypesTree(model, treeIndex);
        modelTree.push({
            key: modelId + ".objectTypes",
            "data-testid": modelId + ".objectTypes",
            type: "objectTypes",
            label: "Object Types",
            propertyName: ["objectTypes"],
            children: objectTypesTree,
            uri: buildUri([adapterId, modelId, "objectTypes"]),
        });
        treeIndex[modelId + ".objectTypes"] = modelTree[1];

        const propertyTypesTree = buildPropertyTypesTree(model, treeIndex);
        modelTree.push({
            key: modelId + ".propertyTypes",
            "data-testid": modelId + ".propertyTypes",
            type: "propertyTypes",
            label: "Property Types",
            propertyName: ["propertyTypes"],
            children: propertyTypesTree,
            uri: buildUri([adapterId, modelId, "propertyTypes"]),
        });
        treeIndex[modelId + ".propertyTypes"] = modelTree[2];

        const tree = {
            key: modelId,
            "data-testid": modelId,
            type: "model",
            label: modelId,
            children: modelTree,            
            uri: buildUri([adapterId, modelId]),
        };
        treeIndex[modelId] = tree;

        dispatch({ type: "RELOAD_TREE", tree: [ tree ], modelTree, treeIndex });

    };


    /*
     * This will fire off when a Tree node is selected.  What is selected will
     * determine which content is rendered in the main content view.
     */
    const onSelectTreeNode = (node) => {

        dispatch({ type: "SELECT_TREE_NODE", node });

        /* When the user selects a node, we need to keep the route and breadcrumbs in sync */
        if (node.uri)
            history.push(node.uri);
    };


    /* This handles the action when user right-clicks on a Tree node  */
    const onContextMenu = (event, node) => {        
        event.preventDefault();
        event.stopPropagation();

        dispatch({ type: "ON_CONTEXT_MENU", node, event });        
    };

    return (
        <>
            <SplitPane                
                style={{ position: undefined, height: "100%" }} 
                split="vertical"
                minSize={100}
                defaultSize={defaultPanelSize}
                onChange={debounce((size) => dispatch({ type: "RESIZE", size }), 200)}
            >
                <div style={{ padding: theme.spacing(0.5), height: "100%", overflow: "auto" }}>                
                    <Tree 
                        data-testid="admin-admin-models-tree"
                        aria-label={modelId}
                        children={tree}   
                        onSelectTreeNode={onSelectTreeNode}                    
                        selectionMode="single"
                        expandedKeys={expandedKeys}
                        onNodeToggle={(node) => dispatch({ type: "TOGGLE_TREE_NODE", node })}
                        selectedKeys={selectedKeys}
                        onContextMenu={editable ? onContextMenu : undefined}
                    />
                </div>
                <div style={{ padding: theme.spacing(0.5), height: "100%", width: "100%", overflow: "auto" }}>
                    <ModelTreeView 
                        model={model}
                        onReloadTree={onReloadTree}
                        selectedNode={selectedNode}
                    />
                </div>
            </SplitPane>
            <ModelTreeContextMenu 
                model={model}
                contextMenuNode={contextMenuNode}
                contextMenuClientX={contextMenuClientX}
                contextMenuClientY={contextMenuClientY}
                onReloadTree={onReloadTree}
                onClose={() => dispatch({ type: "CLOSE_CONTEXT_MENU" })}
            />
        </>
    );
};

export default ModelTree;
