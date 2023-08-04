// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog,
    Spinner,
    Typography,
    useModel
} from "@afw/react";

import {AfwObject} from "@afw/client";

import {useApplication, useTheme} from "../hooks";

export const ObjectUploadModal = (props) => {

    const [objects, setObjects] = useState([]);
    const [error, setError] = useState();
    const [uploading, setUploading] = useState();
    const [finished, setFinished] = useState(false);

    const {notification} = useApplication();  
    const model = useModel();
    const theme = useTheme();
    
    const onReaderLoad = async (event) => {
        let content;
        let objects = [];              
        const {adaptorId, objectTypeId} = props;        

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

    const onUpload = async () => {
        try {            
            for (let i = 0; i < objects.length; i++) {
                const object = objects[i];
                setUploading("Uploading object " + i + " of " + objects.length);

                await object.add().result();
            }       
            
            setUploading();            
        } catch (error) {
            notification({ type: "error", message: error });

            setUploading();            
        }        

        setFinished(true);
        setObjects([]);
    };

    const {open, onDismiss} = props;

    let html5Compatible = false;
    if (window.File && window.FileList && window.FileReader) {
        html5Compatible = true;
    }

    return (
        <Dialog 
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            title="Upload Objects"
            maxWidth="md"
            showClose={true}
            contains={
                <div style={{ padding: theme.spacing(2) }}>                                                
                    <Typography 
                        size="6"
                        text="Select a file containing objects to upload."
                    />                     
                    {
                        html5Compatible ?
                            <div style={{ padding: theme.spacing(2) }}>
                                <input type="file" name="" id="file" onChange={onSelectedFile} />
                                <div style={{ height: theme.spacing(1) }} />
                                <Typography 
                                    size="1"
                                    text="The file should be a JSON-formatted array of adaptive objects."
                                />
                            </div> :
                            <Typography 
                                size="6"
                                text="Your browser does not support HTML5, which is required to upload objects."
                            />
                    }                                                               
                    {
                        uploading ? 
                            <Spinner label={uploading} /> :
                            (objects.length) > 0 && (
                                <div>
                                    <Typography 
                                        size="5"
                                        text={"Found " + objects.length + " Object(s)."}
                                    />
                                </div>
                            )
                    }
                    {
                        finished &&
                            <div>
                                <Typography 
                                    size="5"
                                    text={"Upload Finished."}
                                />
                            </div>
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
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button                                   
                            label="Cancel"
                            size="small"                                    
                            onClick={props.onDismiss}
                            disabled={uploading}
                        />
                    </div>
                    <div>
                        <Button                         
                            label="Upload"
                            size="small"       
                            color="primary"          
                            variant="contained"                   
                            onClick={onUpload}
                            disabled={objects.length === 0}
                        />
                    </div>
                </div>
            }
        />            
    );
};

export default ObjectUploadModal;
