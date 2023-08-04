// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Menu,
} from "@afw/react";


/**
 * This component displays the Toolbar that sits on top of the Objects Table page.  It renders
 * icon buttons for various tasks, along with a Menu. 
 */
export const ObjectsTableToolbar = (props) => {    

    const [optionsMenuTarget, setOptionsMenuTarget] = useState();

    const { 
        sortableProperties, 
        onBatchEdit, 
        onDelete, 
        onUpload, 
        onDownload,
        onRefresh,
        onSortBy,
        objectTypeObject, 
        setShowNewObject,
        selectedObjects,
        viewPropertyMenu, 
    } = props;    

    return (      
        <>           
            <div style={{ display: "flex" }}>
                <Button                                         
                    key="OptionsButton"
                    type="icon"
                    onClick={(event) => {
                        setOptionsMenuTarget(event.currentTarget);                    
                    }}
                    icon="more_vert"
                    color="primary"
                    tooltip="Objects Options"
                    label="More Options"
                />
                <Menu                                        
                    key="OptionsMenu"
                    target={optionsMenuTarget}
                    open={Boolean(optionsMenuTarget)}
                    onClose={() => setOptionsMenuTarget()}
                    color="primary"
                    items={[ 
                        {
                            key: "new",
                            label: "New",
                            onClick: () => {
                                setShowNewObject(true);
                                setOptionsMenuTarget();
                            },
                            disabled: (objectTypeObject?.getPropertyValue("allowAdd") === false)
                        },
                        {
                            key: "edit",
                            label: "Edit",
                            onClick: () => {
                                onBatchEdit();
                                setOptionsMenuTarget();
                            },
                            disabled: (selectedObjects.length === 0)
                        },
                        {
                            key: "delete",
                            label: "Delete",
                            onClick: () => {
                                onDelete();
                                setOptionsMenuTarget();
                            },
                            disabled: (!(selectedObjects.length > 0) || (objectTypeObject?.getPropertyValue("allowDelete") === false))
                        },
                        {
                            type: "divider",
                            key: "divider",
                        },
                        {
                            key: "upload",
                            label: "Upload",
                            onClick: () => {
                                onUpload();
                                setOptionsMenuTarget();
                            },
                            disabled: (objectTypeObject?.getPropertyValue("allowAdd") === false)
                        },
                        {
                            key: "download",
                            label: "Download",
                            onClick: () => {
                                onDownload();
                                setOptionsMenuTarget();
                            },
                            disabled: (selectedObjects.length === 0)
                        },
                        {
                            key: "refresh",
                            label: "Refresh",
                            onClick: () => {
                                onRefresh();
                                setOptionsMenuTarget();
                            }
                        },
                        {
                            key: "divider2",
                            type: "divider"
                        },
                        {
                            key: "sort",
                            label: "Sort By",
                            disabled: (sortableProperties.length === 0),
                            subMenu: {
                                items: sortableProperties.map(property => ({
                                    key: property.getName(),
                                    label: property.getLabel(),
                                    onClick: () => {
                                        onSortBy(property);
                                        setOptionsMenuTarget();
                                    }
                                }))
                            }
                        },
                        viewPropertyMenu()
                    ]}
                />  
                <Button                    
                    type="icon"
                    color="primary"
                    icon="refresh"
                    onClick={onRefresh}
                    tooltip="Refresh Objects Table"
                    label="Refresh Objects Table"
                />
                <Button 
                    type="icon"
                    color="primary"
                    icon="get_app"
                    onClick={onDownload}
                    disabled={selectedObjects.length === 0}
                    tooltip="Download Objects"
                    label="Download Objects"
                /> 
                <Button 
                    type="icon"
                    color="primary"
                    icon="publish"
                    onClick={onUpload}
                    disabled={(objectTypeObject?.getPropertyValue("allowAdd") === false)}
                    tooltip="Upload Objects"
                    label="Upload Objects"
                />  
                <Button 
                    type="icon"
                    color="primary"
                    icon="add"
                    onClick={() => setShowNewObject(true)}
                    disabled={objectTypeObject?.getPropertyValue("allowAdd") === false}
                    tooltip="Create New Object"
                    label="Create New Object"
                /> 
                <Button 
                    type="icon"
                    color="primary"
                    icon="edit"
                    disabled={selectedObjects.length === 0 || (objectTypeObject?.getPropertyValue("allowChange") === false)}
                    onClick={onBatchEdit}
                    tooltip="Batch Edit Object(s)"
                    label="Batch Edit Object(s)"
                />
                <Button 
                    type="icon"
                    color="primary"
                    icon="delete"                    
                    disabled={(!(selectedObjects.length > 0) || (objectTypeObject?.getPropertyValue("allowDelete") === false))}
                    onClick={onDelete}
                    tooltip="Delete Object(s)"
                    label="Delete Object(s)"
                />                           
            </div>
        </>
    );        
};

export default ObjectsTableToolbar;
