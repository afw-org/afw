// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import {Route, Switch} from "react-router";

import {
    Button,
    Link,
    ObjectResponsive,
    ObjectEditor,
    Table,
    Tabs,
    useAfwObject
} from "@afw/react";

import {useTheme} from "../../hooks";
import {PropertyType} from "./PropertyType";

export const PropertiesTable = (props) => {

    const theme = useTheme();
    const [selected, setSelected] = useState([]);
    useAfwObject(props);

    const {allowChange, adapterId, editable, objectTypeId, properties} = props;

    return (
        <div>
            { editable && (
                <div style={{ display: "flex" }}>
                    <div>
                        <Button 
                            label="New"
                            variant="text"
                            color="primary"
                            size="small"
                            icon="add"
                            disabled={!allowChange}
                        />
                    </div>
                    <div style={{ marginLeft: theme.spacing(1) }}>
                        <Button 
                            label="Remove"
                            variant="text"
                            color="primary"
                            size="small"
                            icon="remove"
                            disabled={selected.length === 0 || !allowChange}
                        />
                    </div>
                    <div style={{ height: theme.spacing(2) }} />
                </div>)
            }
            <div>            
                <Table 
                    rows={properties ? properties : []}
                    columns={[
                        {
                            key: "propertyType", 
                            name: "Property Type", 
                            isResizable: true, 
                            minWidth: 100,
                            maxWidth: 200, 
                            width: "20%", 
                            style: { wordWrap: "break-word" },
                            onRender: propertyType => {
                                const propertyName = propertyType.getName();

                                return (
                                    <Link 
                                        text={propertyName}
                                        uriComponents={["Admin", "Schema", adapterId, objectTypeId, propertyName]}
                                    />
                                );
                            }
                        },
                        {
                            key: "description", 
                            name: "Description", 
                            isResizable: true, 
                            minWidth: 100, 
                            maxWidth: 400,
                            isMultiline: true,
                            width: "80%", 
                            style: { wordWrap: "break-word" },
                            onRender: propertyType => propertyType.getValue().getPropertyValue("description")?.replaceAll("\\n", "\n")
                        }
                    ]}
                    selectionMode={editable ? "multiple" : "none"}
                    onSelectionChanged={selected => setSelected(selected)}
                />
            </div>
        </div>
    );
};

export const ObjectType = (props) => {

    const {adapterId, objectTypeId, objectTypeObject, allowChange} = props;

    let properties;
    if (objectTypeObject && objectTypeObject.getPropertyValue("propertyTypes"))
        properties = objectTypeObject.getPropertyValue("propertyTypes").getProperties();

    if (properties)
        properties = properties.sort((A, B) => {
            return A.getName().toLowerCase().localeCompare(B.getName().toLowerCase());
        });

    return (
        <ObjectEditor 
            object={objectTypeObject}
            displayHeader={allowChange}
            layout={
                <Switch>
                    <Route exact path="/Admin/Schema/:adapterId/:objectTypeId" render={() => 
                        <Tabs 
                            gapSpace={20}
                            tabs={[
                                {
                                    text: "General",
                                    contains: 
                                        <ObjectResponsive 
                                            filterOptions={{
                                                filterValuedProperties: true,
                                                filterExcludeProperties: [
                                                    "propertyTypes"
                                                ]
                                            }}
                                        />
                                },
                                {
                                    text: "Properties",
                                    contains: 
                                        <PropertiesTable 
                                            allowChange={allowChange}
                                            adapterId={adapterId}
                                            objectTypeId={objectTypeId}
                                            properties={properties}
                                        />
                                }
                            ]}
                        />
                    } />
                    <Route path="/Admin/Schema/:adapterId/:objectTypeId/:propertyName" render={(routeProps) => {
                        const propertyName = routeProps.match.params.propertyName;
                        let propertyTypeObject;

                        if (objectTypeObject.getPropertyValue("propertyTypes")) {
                            objectTypeObject.getPropertyValue("propertyTypes").getProperties().forEach(p => {
                                if (p.getName() === propertyName)
                                    propertyTypeObject = p.getValue();
                            });
                        }

                        return (
                            <PropertyType 
                                {...props}
                                propertyName={propertyName}
                                propertyTypeObject={propertyTypeObject}
                            />
                        );
                    }} />
                </Switch>
            }
        />
    );
};

export default ObjectType;
