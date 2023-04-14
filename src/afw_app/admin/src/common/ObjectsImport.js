// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog,
    Typography,
    useModel
} from "@afw/react";

import {AfwObject} from "@afw/client";

import {useTheme} from "../hooks";

export const ObjectsImportModal = (props) => {

    const theme = useTheme();
    const [objects, setObjects] = useState([]);
    const [error, setError] = useState();

    const model = useModel();
    const {open, onImport, onDismiss, adaptorId, objectTypeId, prompt, title} = props;
    
    const onReaderLoad = async (event) => {
        let content;
        let objects = [];                  

        try {
            content = JSON.parse(event.target.result);
            
            //! \fixme await initialize() on objects
            if (typeof(content) === "object" && content.constructor === Array) {
                content.forEach(obj => {
                    objects.push( new AfwObject({ model, adaptorId, objectTypeId, object: obj }) );
                });
            } else if (typeof(content) === "object") {
                /* single object */
                objects.push( new AfwObject({ model, adaptorId, objectTypeId, object: content }) );
            }
        } catch (error) {
            /* invalid JSON */
            setError(error);
        }

        setObjects(objects);
    };

    const onSelectedFile = (event) => {
        let reader = new FileReader();

        reader.onload = onReaderLoad;
        reader.readAsText(event.target.files[0]);
    };

    let html5Compatible = false;
    if (window.File && window.FileList && window.FileReader) {
        html5Compatible = true;
    }

    return (
        <Dialog 
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            title={title ? title : "Upload Objects"}
            maxWidth="md"
            showClose={true}
            contains={
                <div style={{ padding: theme.spacing(2) }}>                                                
                    <Typography 
                        size="6"
                        text={prompt ? prompt : "Select a file containing object(s) to upload."}
                    />                     
                    {
                        html5Compatible ?
                            <div style={{ padding: theme.spacing(2) }}>
                                <input type="file" name="" id="upload-file" onChange={onSelectedFile} />
                                <div style={{ height: theme.spacing(1) }} />
                                <Typography 
                                    size="1"
                                    text="The file should be a JSON-encoded object, or array of JSON-encoded objects."
                                />
                            </div> :
                            <Typography 
                                size="6"
                                text="Your browser does not support HTML5, which is required to upload objects."
                            />
                    }     
                    {
                        error && 
                            <Typography 
                                size="5"
                                text={"Error parsing input file:  " + error}
                            />
                    }                        
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <Button 
                        style={{ marginRight: theme.spacing(1) }}
                        label="Cancel"
                        size="small"                                    
                        onClick={onDismiss}
                    />
                    <Button                         
                        label="Import"
                        size="small"       
                        color="primary"
                        variant="contained"                             
                        onClick={() => onImport(objects)}
                        icon="publish"
                        disabled={objects.length === 0}
                    />                    
                </div>
            }
        />            
    );
};

export default ObjectsImportModal;
