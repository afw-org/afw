// See the 'COPYING' file in the project root for licensing information.
import {useState, useCallback} from "react";

import {afwAddObject} from "@afw/core";

import {
    Button,    
    Dialog,
    TextField,
    Typography,
} from "@afw/react";

import {
    VfsTree,
    LocalStorageTree,
} from "./OpenFileDialog";

import {useApplication, useTheme} from "../../hooks";


export const SaveFileDialog = ({ open, onDismiss, vfsAdaptors, storage, onSaveLocal, onSaveVfs }) => {

    const [fileName, setFileName] = useState();
    const [selectedLocalFiles, setSelectedLocalFiles] = useState([]);
    const [selectedVfsFiles, setSelectedVfsFiles] = useState([]);
    const [selectedLocalNode, setSelectedLocalNode] = useState();
    const [selectedVfsNode, setSelectedVfsNode] = useState();
    const [showCreateFolder, setShowCreateFolder] = useState();
    const [folder, setFolder] = useState();
    const [error, setError] = useState();
    const [newFolder, setNewFolder] = useState();

    const theme = useTheme();
    const {client} = useApplication();

    const onCreateFolder = useCallback(async () => {

        try {
            await afwAddObject(client, 
                "_AdaptiveFile_vfs",
                {},
                selectedVfsNode.adaptorId,
                selectedVfsNode.vfsPath + "/" + folder + "/",                
            ).result();
            setNewFolder({ node: selectedVfsNode, folder });
        } catch (e) {
            setError(e);
        }
    }, [client, folder, selectedVfsNode]);

    return (
        <>
            <Dialog 
                open={open}
                title="Save File"
                isBlocking={true}
                onDismiss={onDismiss}    
                maxWidth="lg"
                showClose={true}
                contains={   
                    <div style={{ height: "50vh", padding: theme.spacing(1), display: "flex", flexDirection: "column" }}>                        
                        <TextField 
                            label="File Name"
                            description="The unique name for this file."
                            value={fileName}
                            onChanged={setFileName}
                        />
                        <div style={{ height: theme.spacing(3) }} />
                        <LocalStorageTree 
                            storage={storage}
                            allowSelectDirectories={true}
                            selectedLocalFiles={selectedLocalFiles}
                            onSelectFiles={(selectedFiles, selectedNode) => {
                                if (!selectedNode.isDirectory)
                                    setFileName(selectedNode.label);
                                setSelectedLocalFiles(selectedFiles);
                                setSelectedVfsFiles([]);
                                setSelectedLocalNode(selectedNode);
                                setSelectedVfsNode();
                            }}
                        />
                        <VfsTree 
                            vfsAdaptors={vfsAdaptors}
                            allowSelectDirectories={true}
                            selectedFiles={selectedVfsFiles}
                            newFolder={newFolder}
                            onSelectFiles={(selectedFiles, selectedNode) => {
                                if (!selectedNode.isDirectory)
                                    setFileName(selectedNode.label);
                                setSelectedVfsFiles(selectedFiles);
                                setSelectedLocalFiles([]);
                                setSelectedVfsNode(selectedNode);
                                setSelectedLocalNode();
                            }}
                        />
                    </div>
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>  
                        <div>
                            <Button 
                                variant="contained"
                                color="primary"
                                label="New Folder"
                                icon="add"
                                disabled={!(selectedVfsNode && selectedVfsNode.isDirectory)}
                                onClick={() => setShowCreateFolder(true)}
                            />
                        </div>                  
                        <div style={{ display: "flex" }}>
                            <div>
                                <Button 
                                    variant="contained"
                                    color="primary"
                                    label="Save"
                                    disabled={!fileName || (!selectedLocalNode && !selectedVfsNode)}
                                    onClick={() => {               
                                        if (selectedLocalNode)
                                            onSaveLocal(fileName, selectedLocalNode);
                                        else if (selectedVfsNode)
                                            onSaveVfs(fileName, selectedVfsNode);
                                    }}
                                />
                            </div>
                            <div style={{ marginLeft: theme.spacing(1) }}>
                                <Button
                                    label="Cancel"
                                    onClick={() => {
                                        setFileName();
                                        onDismiss();
                                    }}
                                />
                            </div>   
                        </div>                         
                    </div>
                }
            />
            <Dialog 
                open={Boolean(showCreateFolder)}
                title="New Folder"
                isBlocking={false}
                onDismiss={() => setShowCreateFolder()}    
                maxWidth="sm"
                showClose={true}
                contains={
                    <>
                        <TextField 
                            label="Folder Name"
                            description="The unique name for this folder."
                            value={folder}
                            onChanged={setFolder}
                            // eslint-disable-next-line jsx-a11y/no-autofocus
                            autoFocus={true}
                        />
                        {
                            error && 
                                <Typography color="error" text={error.message} />
                        }
                    </>
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>  
                        <div>
                            <Button 
                                variant="contained"
                                color="primary"
                                label="Create"
                                disabled={!folder}
                                onClick={() => {                                    
                                    onCreateFolder();
                                    setShowCreateFolder();
                                }}
                            />
                        </div>
                        <div style={{ marginLeft: theme.spacing(1) }}>
                            <Button
                                variant="contained"
                                label="Cancel"
                                onClick={() => {
                                    setFolder();
                                    setShowCreateFolder();
                                }}
                            />
                        </div>  
                    </div>
                }   
            />
        </>
    );
};

export default SaveFileDialog;
