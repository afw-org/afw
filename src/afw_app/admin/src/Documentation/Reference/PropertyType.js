// See the 'COPYING' file in the project root for licensing information.
import {
    Checkbox,
    Link,
    Typography
} from "@afw/react";

import {useTheme} from "../../hooks";

export const PropertyType = (props) => {

    const theme = useTheme();
    const {propertyName, propertyType, adapterId, objectTypeObject} = props;

    const {
        description, originURI, referenceURI, collectionURIs, 
        label, dataType, dataTypeParameter, required, unique,
        defaultValue, constraint, testDataParameter,
        allowQuery, allowWrite, possibleValues, tags, hints
    } = propertyType.getPropertyValues();

    return (
        <div>
            <div style={{ paddingBottom: theme.spacing(4) }}>
                <Typography size="9" color="primary" text={propertyName} />
            </div>
            { dataType &&
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <div>                        
                        <Typography color="textSecondary" style={{ display: "inline-block", paddingRight: theme.spacing(0.5) }} text={description + "  This property is a "} />
                        <Link 
                            style={{ display: "inline-block" }}
                            url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(dataType)} 
                            text={dataType + (dataTypeParameter ? "(" + dataTypeParameter + ")" : "")} 
                        />
                        <Typography style={{ display: "inline-block", paddingLeft: theme.spacing(0.5), paddingRight: theme.spacing(0.5) }} text={testDataParameter ? (" with a Test Data Parameter of \"" + testDataParameter + "\".") : "."} />
                        <Typography style={{ paddingRight: theme.spacing(0.5) }} text={defaultValue ? "  When no value is specified, a default value of " + defaultValue + " will be used." : ""} />
                    </div>                    
                </div>
            }
            { label && 
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <div style={{ paddingBottom: theme.spacing(2) }}>
                        <Typography size="6" color="primary" text="Label" />
                    </div>
                    <div style={{ paddingLeft: theme.spacing(2) }}>
                        <Typography text={label} />                        
                    </div>                    
                </div>
            }
            {
                collectionURIs && (
                    <div style={{ paddingBottom: theme.spacing(2) }}>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <Typography size="6" color="primary" text="Collection URIs" />
                        </div>
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <Typography text={collectionURIs} />
                        </div>
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
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Typography size="6" color="primary" text="Constraints" />
                </div>
                <div style={{ paddingLeft: theme.spacing(2) }}>
                    <div style={{ display: "inline-block", paddingRight: theme.spacing(4) }}>
                        <div>
                            <Checkbox label="Required" value={required} disabled={true} />
                        </div>
                        <div>
                            <Checkbox label="Unique" value={unique} disabled={true} />
                        </div>
                    </div>
                    <div style={{ display: "inline-block" }}>
                        <div>
                            <Checkbox label="Queryable" value={allowQuery} disabled={true} />
                        </div>
                        <div>
                            <Checkbox label="Writable" value={allowWrite} disabled={true} />
                        </div>
                    </div>
                    {
                        constraint && <Typography text="This property has a custom constraint." />
                    }                        
                </div>
            </div>
            {
                hints && (
                    <div style={{ paddingBottom: theme.spacing(2) }}>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <Typography size="6" color="primary" text="Hints" />
                        </div>
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <Typography text={hints} />
                        </div>
                    </div>
                )
            }
            {
                possibleValues && (
                    <div style={{ paddingBottom: theme.spacing(2) }}>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <Typography size="6" color="primary" text="Possible Values" />
                        </div>
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <Typography text={possibleValues} />
                        </div>
                    </div>
                )
            }
            {
                tags && (
                    <div style={{ paddingBottom: theme.spacing(2) }}>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <Typography size="6" color="primary" text="Tags" />
                        </div>
                        <div style={{ paddingLeft: theme.spacing(2) }}>
                            <Typography text={tags} />
                        </div>
                    </div>
                )
            }
            <div style={{ paddingBottom: theme.spacing(2) }}>
                <div>
                    <Typography size="6" color="primary" text="Related Object Types" />
                </div>
                <div>
                    <ul>
                        {
                            <li>                                
                                <Link 
                                    url={"/Documentation/Reference/Schema/" + encodeURIComponent(adapterId) + "/" + encodeURIComponent(objectTypeObject.getObjectId())} 
                                    text={objectTypeObject.getObjectId()} />
                            </li>
                        }
                    </ul>
                </div>
            </div>
        </div>
    );
};

export default PropertyType;
