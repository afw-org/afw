// See the 'COPYING' file in the project root for licensing information.
import {
    Typography,
    Divider,
    Paper,
    Tabs
} from "@afw/react";

import {useTheme} from "../../hooks";

const Spacer = () => {
    const theme = useTheme();

    return <div style={{ height: theme.spacing(2) }} />;
    
};

export const SchemaApiRest = (props) => {

    const theme = useTheme();
    const {adaptorId, objectTypeObject} = props;

    return (
        <div style={{ padding: theme.spacing(2) }}>
            <Typography style={{ fontFamily: "Roboto mono" }} text={"GET /" + adaptorId + "/" + objectTypeObject.getObjectId() + "/"} />
            <Spacer />
            <Typography size="3" text={"Retrieves object instances of type " + objectTypeObject.getObjectId() + " from the adaptor " + adaptorId + "."} />

            <Spacer />
            <Divider />
            <Spacer />

            <Typography style={{ fontFamily: "Roboto mono" }} text={"GET /" + adaptorId + "/" + objectTypeObject.getObjectId() + "/{objectId}"} />
            <Spacer />
            <Typography size="3" text={"Gets an object instances of type " + objectTypeObject.getObjectId() + ", identified by objectId, from the adaptor " + adaptorId + "."} />

            <Spacer />
            <Divider />
            <Spacer />

            <Typography style={{ fontFamily: "Roboto mono" }} text={"PUT /" + adaptorId + "/" + objectTypeObject.getObjectId() + "/{objectId}"} />
            <Spacer />
            <Typography size="3" text={"Creates a new object of type " + objectTypeObject.getObjectId() + ", with an optional objectId, in the adaptor " + adaptorId + "."} />

            <Spacer />
            <Divider />
            <Spacer />

            <Typography style={{ fontFamily: "Roboto mono" }} text={"DELETE /" + adaptorId + "/" + objectTypeObject.getObjectId() + "/{objectId}"} />
            <Spacer />
            <Typography size="3" text={"Deletes an existing object of type " + objectTypeObject.getObjectId() + " in the adaptor " + adaptorId + "."} />

            <Spacer />
            <Divider />
            <Spacer />

            <Typography style={{ fontFamily: "Roboto mono" }} text={"POST /" + adaptorId + "/" + objectTypeObject.getObjectId() + "/{objectId}"} />
            <Spacer />
            <Typography size="3" text={"Modifies an existing object of type " + objectTypeObject.getObjectId() + ", identified by objectId, in the adaptor " + adaptorId + "."} />
        </div>
    );
};

export const SchemaApiPerform = (props) => {

    const theme = useTheme();
    const {adaptorId, objectTypeObject} = props;

    return (
        <div style={{ padding: theme.spacing(2) }}>
            <Typography style={{ fontFamily: "Roboto mono" }} text="POST /afw" />
            <Typography size="3" text="All Perform operations use HTTP POST to the core endpoint, /afw.  The payload specifies the action for all Perform API requests." />
            <Spacer />
            <Typography text="Payload:" />
            <Typography style={{ fontFamily: "Roboto mono", whiteSpace: "pre-wrap" }} text={`{
    "functionId": "retrieve_objects",
    "adaptorId": "${adaptorId}",
    "objectType": "${objectTypeObject.getObjectId()}"
} `} />
            <Spacer />
            <Typography size="3" text={"Retrieves object instances of type " + objectTypeObject.getObjectId() + " from the adaptor " + adaptorId + "."} />   
            <Spacer />

            <Typography text="Response:" />
            <Typography style={{ fontFamily: "Roboto mono", whiteSpace: "pre-wrap" }} text={`{
    "status": "success",
    "result": [
        
    ]
} `} /> 

            <Spacer />
            <Divider />
            <Spacer />

            <Typography text="Payload:" />
            <Typography style={{ fontFamily: "Roboto mono", whiteSpace: "pre-wrap" }} text={`{
    "functionId": "get_object",
    "adaptorId": "${adaptorId}",
    "objectType": "${objectTypeObject.getObjectId()}",
    "objectId": "{objectId}"
} `} />
            <Spacer />
            <Typography size="3" text={"Gets an object instances of type " + objectTypeObject.getObjectId() + ", identified by objectId, from the adaptor " + adaptorId + "."} />   
            <Spacer />

            <Typography text="Response:" />
            <Typography style={{ fontFamily: "Roboto mono", whiteSpace: "pre-wrap" }} text={`{
    "status": "success",
    "result": {
        
    }
} `} /> 

            <Spacer />
            <Divider />
            <Spacer />

        </div>
    );
};

export const SchemaApi = (props) => {

    const theme = useTheme();
    const {adaptorId, objectTypeObjects} = props;

    const sortedObjectTypes = Object.entries(objectTypeObjects)
        .sort((A, B) => A[0].toLowerCase().localeCompare(B[0].toLowerCase()))
        .map(([, objectType]) => objectType)
        .filter(o => o.getObjectId() !== "_AdaptiveObjectType_" && o.getPropertyValue("allowEntity") !== false);

    return (
        <>
            <Typography size="8" text={"API Reference for " + adaptorId} color="primary" />
            <div style={{ height: theme.spacing(5) }} />
            {
                sortedObjectTypes.map( (objectTypeObject, index) => {                    
                    return (
                        <div key={index}>
                            <Paper 
                                contains={
                                    <div style={{ padding: theme.spacing(2) }}>
                                        <Typography size="7" text={objectTypeObject.getObjectId()} />
                                        <Tabs 
                                            gapSpace={20}
                                            tabs={[
                                                {
                                                    text: "REST",
                                                    contains: <SchemaApiRest {...props} objectTypeObject={objectTypeObject} />
                                                },
                                                {
                                                    text: "Perform",
                                                    contains: <SchemaApiPerform {...props} objectTypeObject={objectTypeObject} />
                                                }
                                            ]}
                                        />    
                                    </div>       
                                }    
                            />    
                            <div style={{ height: theme.spacing(6) }} />
                        </div>
                    );
                })
            }
        </>
    );
};

export default SchemaApi;
