// See the 'COPYING' file in the project root for licensing information.
import {useTheme, useClasses} from "../../hooks";

import { 
    Divider,   
    Icon,
    Spinner,
    Typography
} from "@afw/react";


const styles = theme => ({
    table: {
        display: "table",
        padding: theme.spacing(2)
    },
    row: {
        display: "table-row",
        padding: theme.spacing(1)
    },
    cellNoWrap: {
        display: "table-cell",
        padding: theme.spacing(0.5),
        whiteSpace: "nowrap"
    },
    cell: {
        display: "table-cell",
        padding: theme.spacing(0.5)
    }
});

const HelpText = ({ text }) => {
    return (
        <Typography 
            size="3"
            style={{ whiteSpace: "pre-line" }}
            paragraph={true}
            text={text}
        />
    );
};

const PossibleValuesHelp = ({ propertyType }) => {

    return propertyType.possibleValues ? (
        <>
            <HelpText text="Possible Values: " />
            <ul>
                {
                    propertyType.possibleValues.map(v =>
                        <li key={v}>{ v }</li>
                    )
                }
            </ul>                                                                                        
        </>
    ) : null;
};

const PropertyScriptReturnsHelp = ({ propertyType, objectTypeObject }) => {

    const theme = useTheme();

    return (
        <div style={{ marginLeft: theme.spacing(3) }} key={propertyType}>                                               
            <Typography color="primary" size="5" text={propertyType.label} />
            <HelpText text={propertyType.description} />         
            <HelpText text="This property returns an Object with the following properties:" />         
            {
                Object.entries(objectTypeObject.propertyTypes).map(([_scriptPropertyName, _scriptPropertyType]) => {
                    if (_scriptPropertyName === "_meta_")
                        return null;

                    return (
                        <div style={{ marginLeft: theme.spacing(3) }} key={_scriptPropertyName}>
                            <Typography color="primary" size="5" text={_scriptPropertyType.label} />
                            <HelpText text={_scriptPropertyType.description} />
                            <PossibleValuesHelp propertyType={_scriptPropertyType} />
                        </div>
                    );
                })
            }                                                                                     
        </div>
    );
};

const PropertyScriptHelp = ({ object, property }) => {

    const [, scriptObjectType] = property.getDataTypeParameter().split(" ");
    const adapterId = object.getAdapterId();
    const model = object.getModel();
    const theme = useTheme();

    const objectTypeObject = model.getObjectTypeObject({ adapterId, objectTypeId: scriptObjectType });

    return (
        <div key={property.getName()} style={{ marginTop: theme.spacing(2) }}>                            
            <Typography color="primary" size="5" text={property.getLabel()} />
            <HelpText text={property.getDescription()} />
            <Typography color="primary" size="5" text="Returns" />
            {
                scriptObjectType && (
                    <div style={{ marginLeft: theme.spacing(3) }}>
                        {
                            objectTypeObject.propertyTypes && Object.entries(objectTypeObject.propertyTypes).map(([_propertyName, _propertyType]) => {
                                return (
                                    <div style={{ marginLeft: theme.spacing(3) }} key={_propertyName}>
                                        <Typography color="primary" size="5" text={_propertyType.label} />
                                        <HelpText text={_propertyType.description} />
                                    </div>
                                );
                            })
                        }
                    </div>
                )
            }
        </div>
    );
};

const PropertyObjectHelp = ({ object, property }) => {

    const theme = useTheme();

    const objectTypeObject = property.getObjectTypeObject();
    return (
        <div key={property.getName()} style={{ marginTop: theme.spacing(2) }}>
            <Typography color="primary" size="5" text={property.getLabel()} />
            <HelpText text={property.getDescription()} />
            {
                objectTypeObject.propertyTypes && Object.entries(objectTypeObject.propertyTypes).map(([_propertyName, _propertyType]) => {
                    if (_propertyName === "_meta_")
                        return null;

                    if (_propertyType.dataType === "script" && _propertyType.dataTypeParameter) {
                        const [, scriptObjectType] = _propertyType.dataTypeParameter.split(" ");
                        const adapterId = object.getAdapterId();
                        const model = object.getModel();

                        if (scriptObjectType) {
                            const scriptObjectTypeObject = model.getObjectTypeObject({ 
                                adapterId, objectTypeId: scriptObjectType 
                            });
                            if (scriptObjectTypeObject) {
                                return (
                                    <PropertyScriptReturnsHelp 
                                        propertyType={_propertyType} 
                                        objectTypeObject={scriptObjectTypeObject} 
                                    />
                                );                                     
                            }
                        } 

                        return (
                            <div style={{ marginLeft: theme.spacing(3) }} key={_propertyName}>                                               
                                <Typography color="primary" size="5" text={_propertyType.label} />
                                <HelpText text={_propertyType.description} />  
                                <PossibleValuesHelp propertyType={_propertyType} />                                                  
                            </div>
                        );
                    } else {
                        return (
                            <div style={{ marginLeft: theme.spacing(3) }} key={_propertyName}>                                               
                                <Typography color="primary" size="5" text={_propertyType.label} />
                                <HelpText text={_propertyType.description} />   
                                <PossibleValuesHelp propertyType={_propertyType} />                                                
                            </div>
                        );
                    }                                        
                    
                })
            }    
        </div>
    );
};

const PropertyHelp = ({ object, property }) => {

    const theme = useTheme();

    if (property.getDataType() === "object")
    {
        return <PropertyObjectHelp object={object} property={property} />;
    }

    else if (property.getDataType() === "script" && property.getDataTypeParameter()) 
    {
        return <PropertyScriptHelp object={object} property={property} />;
    }

    else 
    {
        return (
            <div key={property.getName()} style={{ marginTop: theme.spacing(2) }}>                            
                <Typography color="primary" size="5" text={property.getLabel()} />
                <HelpText text={property.getDescription()} />
            </div>
        );
    }

};

const ObjectHelp = ({ header, filterProperties, object, isLoading, error }) => {

    if (isLoading)
        return <Spinner />;

    if (error)
        return <Typography text="Unable to load Application object." />;

    if (!object) 
        return null;

    const properties = object.getProperties().filter(p => 
        filterProperties ? filterProperties.includes(p.getName()) : true);

    return (
        <>
            <Typography color="primary" size="6" text={header} />
            {
                properties.map(property => 
                    <PropertyHelp 
                        key={property.getName()} 
                        object={object} 
                        property={property} 
                    />)
            }
        </>
    );
};

export const ApplicationHelp = (props) => {    

    const theme = useTheme();
    const {table, row, cell, cellNoWrap} = useClasses(styles);

    const {applicationConf, isLoading} = props;

    return (
        <>
            <HelpText text={` \
                Define your Adaptive Application here, which instructs the Adaptive Framework Core which configurations and \
                extensions to load.  You may also define active flags, variables and authorization rules, which will are accessible from  \
                all Adaptive Expressions throughout the Application, including Models, Adapters, Logs and Authorization Policies.  The Adaptive Application \
                is your anchor for everything related to this instance of Adaptive Framework.
                
                The Adaptive Application is simply an Adaptive Object, which can be edited here using the provided UI layout, or you may choose \
                to view and edit it as JSON Source.  Use the toolbar buttons to switch between the two, or select more menu options.
            `} />
        
            <div style={{ padding: theme.spacing(2), display: "flex", justifyContent: "center" }}>
                <Icon style={{ marginRight: theme.spacing(0.5) }} iconName="more_vert" />
                <Icon style={{ marginRight: theme.spacing(0.5) }} iconName="view_quilt" />
                <Icon style={{ marginRight: theme.spacing(2) }} iconName="code" />
                <Icon iconName="lock" />
            </div>

            <HelpText text="The first toolbar button provides a Menu to use to operate on this object." />     

            <div className={table}>
                <div className={row}>
                    <div className={cellNoWrap}>                        
                        <Typography color="primary" text="Save Changes" />    
                    </div>                    
                    <div className={cell}>                        
                        <HelpText text="If you have modified the Application Object, this command will save your changes back out to the server" />
                    </div>
                </div>
                <div className={row}>
                    <div className={cellNoWrap}>
                        <Typography color="primary" text="Review Changes" />    
                    </div>
                    <div className={cell}>
                        <HelpText text="If you have modified the Application Object, this command will allow you to view the differences between your version and the original" />
                    </div>                    
                </div>
                <div className={row}>
                    <div className={cellNoWrap}>
                        <Typography color="primary" text="Discard Changes" />    
                    </div>
                    <div className={cell}>
                        <HelpText text="If you have modified the Application Object, this command will discard any changes you have made and revert the Application Object back to the original copy" />
                    </div>                    
                </div>
                <div className={row}>
                    <div className={cellNoWrap}>
                        <Typography color="primary" text="Go to Object Viewer" />    
                    </div>       
                    <div className={cell}>
                        <HelpText text="This command will open up the current Application Object in the Object Viewer page" />
                    </div>                                 
                </div>
            </div>  

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <Typography color="primary" size="6" text="Editing the Application" />
            <HelpText text={` \
                Enter Edit mode by clicking on the "lock" icon or type the "E" key on your keyboard.

                Once in Edit mode, you may switch between tabs and edit the fields you want to change. \
                When finished, you have the option to Cancel and discard your changes, Save your changes, \
                or Review them.
            `} />

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <ObjectHelp 
                object={applicationConf} 
                isLoading={isLoading} 
                header="General Tab" 
                filterProperties={[
                    "title", "description", "applicationId", "confAdapterId", "defaultAdapterId", "defaultModelAdapterId", "layoutsAdapterId", "rootFilePaths"
                ]} 
            />

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <ObjectHelp 
                object={applicationConf} 
                isLoading={isLoading} 
                header="Startup Tab" 
                filterProperties={[
                    "extensions", "extensionModulePaths", "onApplicationStartupComplete"
                ]} 
            />

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <ObjectHelp 
                object={applicationConf} 
                isLoading={isLoading} 
                header="Flags Tab" 
                filterProperties={[
                    "defaultFlags"
                ]} 
            />

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <ObjectHelp 
                object={applicationConf} 
                isLoading={isLoading} 
                header="Authorization Tab" 
                filterProperties={[
                    "authorizationControl"
                ]} 
            />

            <Divider />
            <div style={{ height: theme.spacing(2) }} />

            <ObjectHelp 
                object={applicationConf} 
                isLoading={isLoading} 
                header="Variables Tab" 
                filterProperties={[
                    "qualifiedVariables"
                ]} 
            />
        </>
    );
};


export const ContextualHelpRoutes = [
    
    { path: "/Admin/Application",   title: "Application Help",  Component: ApplicationHelp  },

];

export default ContextualHelpRoutes;
