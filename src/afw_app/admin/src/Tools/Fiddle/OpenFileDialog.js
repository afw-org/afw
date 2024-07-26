// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";

import {
    Button,
    Dialog,
    Icon,
    Spinner,
    TextField,    
    Tree,
    Typography,
} from "@afw/react";

import {afwGetObjectWithUri, afwRetrieveObjects} from "@afw/core";
import {useApplication, useTheme} from "../../hooks";

/**
 * LocalStorageTree component displays the contents of "scripts" inside of 
 * Local Storage.
 */
export const LocalStorageTree = ({ storage, selectedFiles, allowSelectDirectories = false, onSelectFiles }) => {

    const [expandedKeys, setExpandedKeys] = useState([]);
    const [tree, setTree] = useState([]);

    useEffect(() => {

        let children = [];
        if (storage && storage.scripts) {
            children = Object.keys(storage.scripts).map(file => ({
                key: "/LocalStorage/" + file,
                label: file,
                path: "/LocalStorage/" + file,                
            }));
        }

        setTree([
            {
                key: "LocalStorage",
                label: "Local Storage",
                path: "/LocalStorage/",
                isDirectory: true,
                children,                
            }
        ]);
    }, [storage]);

    const onSelectTreeNode = (node) => {
                        
        if (allowSelectDirectories === false && node.isDirectory)
            onSelectFiles([]);
        else if (selectedFiles && selectedFiles[0] === node.key)
            onSelectFiles([], node);
        else
            onSelectFiles([ node.key ], node);        
    };

    return (
        <Tree 
            selectionMode="single"
            children={tree}  
            expandedKeys={expandedKeys}
            onNodeToggle={setExpandedKeys}
            onSelectTreeNode={onSelectTreeNode}
            defaultExpandIcon={<Icon iconName="chevron_right" />}                 
            defaultCollapseIcon={<Icon iconName="expand_more" />}     
            defaultEndIcon={<Icon iconName="insert_drive_file" />}            
        />
    );
};

/**
 * VfsTree component displays the VFS adapters and their recursive
 * file system in a array of Trees.
 */
export const VfsTree = ({ vfsAdapters, selectedFiles, onSelectFiles, allowSelectDirectories = false, newFolder }) => {

    const [expandedKeys, setExpandedKeys] = useState([]);
    const [tree, setTree] = useState([]);
    const {client} = useApplication();
    const [keyNodeMap, setKeyNodeMap] = useState({});

    const theme = useTheme();

    useEffect(() => {        

        const fetchFiles = async (adapterId, treeItem, keyNodeMap) => {
            
            const response = afwRetrieveObjects(client, 
                "_AdaptiveFile_vfs", 
                adapterId, 
                undefined, 
                { objectId: true, path: true },
            );
            const result = await response.result();

            treeItem.children = result.map(res => {
                const {isDirectory, vfsPath, _meta_: { objectId, path }} = res;

                const treeItem = {
                    key: path, 
                    label: objectId,
                    path,
                    isDirectory,                   
                    adapterId,
                    isLoading: true,
                    vfsPath,
                    children: isDirectory ? [
                        {
                            key: vfsPath + ".isLoading",    
                            icon: "loop",                        
                            contains: <Spinner style={{ float: "left" }} size="small" />,
                            loadingItem: true,
                        }
                    ] : []
                };

                keyNodeMap[path] = treeItem;
                return treeItem;
            });           

            setTree([ treeItem ]);
            setKeyNodeMap(keyNodeMap);
        };

        if (vfsAdapters && client) {
            vfsAdapters.forEach( ({ adapterId }) => {
                const treeItem = {
                    key: "/" + adapterId + "/_AdaptiveFile_vfs/",
                    label: adapterId,
                    path: "/" + adapterId + "/_AdaptiveFile_vfs/",
                    vfsPath: "",
                    contains: 
                        <div style={{ display: "flex" }}>
                            <Typography style={{ marginRight: theme.spacing(3) }} text={adapterId} />
                            <Button type="icon" icon="refresh" size="small" />
                        </div>,
                    adapterId: adapterId,
                    children: [],
                    collapseIcon: "expand_more",                    
                    expandIcon: "chevron_right",
                    isDirectory: true
                };      
                let keyNodeMap = {};
                keyNodeMap["/" + adapterId] = treeItem;

                fetchFiles(adapterId, treeItem, keyNodeMap);
            });            
        }

    }, [client, vfsAdapters, theme]);

    useEffect(() => {
        if (newFolder) {
            const {node, folder} = newFolder;
            if (!node.children)
                node.children = [];

            const key = node.key + "%2F" + folder;
            const treeItem = {
                key,
                path: key,
                label: folder,
                isDirectory: true,
                vfsPath: node.vfsPath + "%2F" + folder,
                adapterId: node.adapterId,
                isLoading: false,
                children: [],
                icon: "folder"
            };
            node.children.push(treeItem);            

            setKeyNodeMap(keyNodeMap => {
                keyNodeMap[key] = treeItem;
                return keyNodeMap;
            });
            setTree(tree => [ ...tree ]);
        }
    }, [newFolder]);
    
    const fetchFiles = async (node) => {
        
        const response = afwGetObjectWithUri(client,
            node.path,
            { objectId: true, path: true }
        );
        const result = await response.result();

        node.children = result.data.map(data => {
            const isDirectory = data.endsWith("/");
            const objectId = isDirectory ? data.substring(0, data.length - 1) : data;
            const treeItem = {
                key: node.key + objectId + (isDirectory ? "%2F" : ""), 
                label: data,
                path: node.path + objectId + (isDirectory ? "%2F" : ""),
                vfsPath: node.vfsPath + "%2F" + objectId,
                isDirectory,
                adapterId: node.adapterId,
                isLoading: true,
                children: isDirectory ? [
                    {
                        key: node.key + data + ".isLoading",    
                        icon: "loop",                        
                        contains: <Spinner style={{ float: "left" }} size="small" />,
                        loadingItem: true,
                    }
                ] : []
            };                                    

            keyNodeMap[node.key + objectId + (isDirectory ? "%2F" : "")] = treeItem;
            return treeItem;
        });
        
        node.isLoading = false;
        if (node.children.length === 0) {
            node.icon = "folder";
        }    

        setTree([ ...tree ]);
        setKeyNodeMap(keyNodeMap);
    };
    
    const onNodeToggle = (keys) => {
        setExpandedKeys(keys);

        /* find key in our tree and figure out if we need to load children */
        const node = keyNodeMap[keys[0]];
        if (node && node.path && node.isLoading) {
            fetchFiles(node);
        }
    };

    const onSelectTreeNode = (node) => {
        if (allowSelectDirectories && node.isDirectory)
            onSelectFiles([ node.key ], node);
        else if (node.isDirectory || node.loadingItem)
            onSelectFiles([]);
        else if (selectedFiles && selectedFiles[0] === node.key)
            onSelectFiles([], node);
        else
            onSelectFiles([ node.key ], node);
    };

    return (
        <Tree 
            selectionMode="single"
            children={tree}
            onNodeToggle={onNodeToggle}
            onSelectTreeNode={onSelectTreeNode}
            selectedKeys={selectedFiles}       
            expandedKeys={expandedKeys}                        
            defaultExpandIcon={<Icon iconName="chevron_right" />}                 
            defaultCollapseIcon={<Icon iconName="expand_more" />}     
            defaultEndIcon={<Icon iconName="insert_drive_file" />}            
        />
    );
};

/**
 * This component is used to browse VFS file adapter to open up files for Fiddle.
 */
export const OpenFileDialog = ({ open, onDismiss, vfsAdapters, onOpen, storage }) => {

    const [selectedLocalFiles, setSelectedLocalFiles] = useState([]);
    const [selectedVfsFiles, setSelectedVfsFiles] = useState([]);
    const [selectedNode, setSelectedNode] = useState();
    const [suffix, setSuffix] = useState("*.*");

    const theme = useTheme();

    const selectedPath = selectedNode ? (selectedNode.vfsPath ? selectedNode.vfsPath : selectedNode.path) : "";

    return (
        <Dialog
            open={open}
            title="Open File"
            isBlocking={true}
            onDismiss={onDismiss}    
            maxWidth="lg"
            showClose={true}
            contains={   
                <div style={{ height: "50vh", padding: theme.spacing(1), display: "flex", flexDirection: "column" }}>                    
                    <div>
                        <TextField 
                            placeholder="Path..."
                            value={selectedPath}
                            disabled
                        />
                        <div style={{ height: theme.spacing(3) }} />                        
                    </div>
                    <div style={{ flex: 1, overflow: "auto", }}>
                        <LocalStorageTree 
                            storage={storage}                            
                            suffix={suffix}
                            selectedFiles={selectedLocalFiles}
                            onSelectFiles={(selectedFiles, selectedNode) => {
                                setSelectedLocalFiles(selectedFiles);
                                setSelectedVfsFiles([]);
                                setSelectedNode(selectedNode);
                            }}
                        />
                        <VfsTree 
                            vfsAdapters={vfsAdapters} 
                            selectedFiles={selectedVfsFiles}
                            onSelectFiles={(selectedFiles, selectedNode) => {
                                setSelectedVfsFiles(selectedFiles);
                                setSelectedLocalFiles([]);
                                setSelectedNode(selectedNode);
                            }}
                            suffix={suffix}
                        />      
                    </div>              
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>
                    <div style={{ display: "flex", visibility: "hidden" }}>
                        <TextField 
                            placeholder="Filename Suffix"
                            value={suffix}
                            onChanged={setSuffix}
                            disabled
                        />      
                        <div style={{ marginLeft: theme.spacing(1) }}>
                            <Button 
                                type="icon"
                                label="Refresh"
                                icon="refresh"
                            />
                        </div>                        
                    </div>
                    <div style={{ display: "flex" }}>
                        <div>
                            <Button 
                                variant="contained"
                                color="primary"
                                label="Open"
                                disabled={!selectedNode}
                                onClick={() => {                                    
                                    if (selectedVfsFiles.length > 0)
                                        onOpen(selectedVfsFiles[0], false);
                                    else
                                        onOpen(selectedLocalFiles[0], true);
                                    setSelectedLocalFiles([]);
                                    setSelectedVfsFiles([]);
                                    setSelectedNode();
                                }}
                            />
                        </div>
                        <div style={{ marginLeft: theme.spacing(1) }}>
                            <Button
                                label="Cancel"
                                onClick={() => {
                                    setSelectedLocalFiles([]);
                                    setSelectedVfsFiles([]);
                                    setSelectedNode();
                                    onDismiss();
                                }}
                            />
                        </div>   
                    </div>                         
                </div>
            }  
        />
    );
};

export default OpenFileDialog;
