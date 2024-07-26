// See the 'COPYING' file in the project root for licensing information.
import {
    Link,
    Table,
    Typography
} from "@afw/react";

import {useTheme} from "../../hooks";

import InheritanceTree from "../../common/InheritanceTree";

const ObjectType = (props) => {

    const theme = useTheme();
    const {adapterId, objectTypeObject, objectTypeObjects} = props;
   
    /* sort the propertyTypes */
    let propertyTypes;

    if (objectTypeObject && objectTypeObject.getPropertyValue("propertyTypes")) {
        propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");
        

        propertyTypes = propertyTypes.getProperties().sort((a, b) => {
            let aName = a.getName().toLowerCase();
            let bName = b.getName().toLowerCase();            

            if (a.getValue().getPropertyValue("required") === true) {
                if (b.getValue().getPropertyValue("required") === true) 
                    return aName < bName ? -1 : 1;
                else
                    return -1;
            } else if (b.getValue().getPropertyValue("required") === true) {
                return 1;
            } else {
                return aName < bName ? -1 : 1;
            }
        });
    }     

    if (!objectTypeObject)
        return null;

    const objectType = objectTypeObject.getObjectId();         

    const {
        collectionURIs, descriptionPropertyName, objectIdPropertyName,
        referenceURI, originURI, description
    } = objectTypeObject.getPropertyValues();
        
    return (
        <div>
            <div>
                <Typography size="8" color="primary" text={objectType} />
                <div style={{ height: theme.spacing(1) }} />
                <Typography color="textSecondary" text={description ? description : ""} />                                                                            
            </div>
            <div style={{ height: theme.spacing(1) }} />
            <div style={{ padding: theme.spacing(1) }}>
                <div style={{ paddingBottom: theme.spacing(4) }}>
                    <Typography size="6" color="primary" text="Inheritance" />    
                    <div style={{ paddingLeft: theme.spacing(2), paddingTop: theme.spacing(2) }}>
                        <Typography size="5" text="Ancestors" />
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <InheritanceTree 
                                object={objectTypeObject}
                                objects={objectTypeObjects}
                                onRenderPath={(path) => {
                                    const paths = path.split("/");
            
                                    if (path === objectTypeObject.getPath())
                                        return (
                                            <Typography text={paths[paths.length -1]} />
                                        );
                                    else
                                        return (
                                            <Link url={"/Documentation/Reference/Schema/" + objectTypeObject.getAdapterId() + "/" + paths[paths.length-1]} text={paths[paths.length - 1]} />
                                        );
                                }}
                            />
                        </div>
                    </div>
                    <div style={{ paddingLeft: theme.spacing(2), paddingTop: theme.spacing(2) }}>
                        <Typography size="5" text="Descendants" />
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <InheritanceTree 
                                object={objectTypeObject}
                                objects={objectTypeObjects}
                                direction="descendants"
                                onRenderPath={(path) => {
                                    const paths = path.split("/");
            
                                    if (path === objectTypeObject.getPath())
                                        return (
                                            <Typography text={paths[paths.length -1]} />
                                        );
                                    else
                                        return (
                                            <Link url={"/Documentation/Reference/Schema/" + objectTypeObject.getAdapterId() + "/" + paths[paths.length-1]} text={paths[paths.length - 1]} />
                                        );
                                }}
                            />
                        </div>
                    </div>
                </div>
                <div style={{ paddingBottom: theme.spacing(2) }}>                          
                    {
                        descriptionPropertyName &&
                            <>
                                <Typography style={{ display: "inline-block" }} text="  Instances of this object type use the " />
                                <Link 
                                    style={{ display: "inline-block", padding: theme.spacing(1) }}
                                    url={"/Documentation/Reference/Schema/" + encodeURIComponent(adapterId) + "/" + encodeURIComponent(objectType) + "/" + encodeURIComponent(descriptionPropertyName)} 
                                    text={descriptionPropertyName} 
                                />
                                <Typography style={{ display: "inline-block" }} text=" property for its object description." />
                            </>                                                              
                    }
                    {
                        objectIdPropertyName &&
                            <>
                                <Typography style={{ display: "inline-block" }} text="  Instances of this object type use the " />
                                <Link 
                                    style={{ display: "inline-block", padding: theme.spacing(1) }}
                                    url={"/Documentation/Reference/Schema/" + encodeURIComponent(adapterId) + "/" + encodeURIComponent(objectType) + "/" + encodeURIComponent(objectIdPropertyName)} 
                                    text={objectIdPropertyName} 
                                />
                                <Typography style={{ display: "inline-block" }}  text=" property for its Object Id." />
                            </>    
                    }                                             
                </div>
                {
                    collectionURIs && (
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div>
                                <Typography size="6" color="primary" text="Collection URIs" />
                            </div>
                            <ul>
                                {
                                    collectionURIs.map((collectionURI, index) => 
                                        <li key={index}>
                                            <Typography text={collectionURI.getValue()} />
                                        </li>
                                    )
                                }
                            </ul>
                        </div>
                    )
                }
                {
                    referenceURI && (
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography size="6" color="primary" text="Reference URI" />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                <Typography text={referenceURI} />
                            </div>
                        </div>
                    )
                }
                {
                    originURI && (
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography size="6" color="primary" text="Origin URI" />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                <Typography text={originURI} />
                            </div>
                        </div>
                    )
                }
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Typography size="6" color="primary" text="Property Types" />
                </div>
                <div style={{ paddingBottom: theme.spacing(5) }}>
                    { propertyTypes &&
                        <Table 
                            rows={propertyTypes}
                            columns={[
                                { 
                                    key: "name", name: "Property", minWidth: 100, maxWidth: 175, isResizable: true,
                                    style: { wordWrap: "break-word" },
                                    onRender: (item) => <Link url={"/Documentation/Reference/Schema/" + adapterId + "/" + objectType + "/" + item.getName()} text={item.getName()} />                                        
                                },
                                                                    
                                { 
                                    key: "dataType", name: "Data Type", minWidth: 75, maxWidth: 150, isResizable: true,
                                    style: { wordWrap: "break-word" },
                                    onRender: (item) => {
                                        let dataType = item.getValue().getPropertyValue("dataType");
                                        let dataTypeParameter = item.getValue().getPropertyValue("dataTypeParameter");
                                        
                                        if (dataTypeParameter) {
                                            if (dataType === "array") {
                                                let uri = dataTypeParameter;
                                                if (dataTypeParameter.indexOf(" ") > 0)
                                                    uri = dataTypeParameter.split(" ")[1];
                                                    
                                                return (
                                                    <>
                                                        <Link 
                                                            style={{ display: "inline-block" }}
                                                            url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(uri)} 
                                                            text={dataTypeParameter} 
                                                        />
                                                        {"[]"}  
                                                    </>
                                                );
                                            } else if (dataType === "object")
                                                return (
                                                    <Link 
                                                        url={"/Documentation/Reference/Schema/" + adapterId + "/" + encodeURIComponent(dataTypeParameter)} 
                                                        text={dataTypeParameter} 
                                                    />
                                                );

                                            return (
                                                <>
                                                    <Link 
                                                        style={{ display: "inline-block" }}
                                                        url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(dataType)} 
                                                        text={dataType} 
                                                    /> 
                                                    {" (" + dataTypeParameter + ")"}                                                    
                                                </>
                                            );
                                        } else return (
                                            <Link 
                                                url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(dataType)} 
                                                text={dataType} 
                                            />
                                        );                                            
                                    }
                                },                                      
                                { 
                                    key: "description", name: "Description", isMultiline: true,
                                    style: { wordWrap: "break-word" },
                                    onRender: (item) => <Typography style={{ whiteSpace: "pre-wrap" }} text={ item.getValue().getPropertyValue("description")?.replaceAll("\\n", "\n") } />
                                } 
                            ]}
                            selectionMode="none"
                        />
                    }
                </div>
            </div>
        </div>
    );
};

export default ObjectType;
