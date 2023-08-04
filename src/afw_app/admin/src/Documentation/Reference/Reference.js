// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useEffect} from "react";
import {Route, Switch, useLocation} from "react-router";

import Container from "../../common/Container";

import {
    Breadcrumb,    
    Divider,
    Link,
    Responsive,
    Spinner,    
    Typography,
    useDataTypes,
    useFunctions,
    useObjectTypes,
} from "@afw/react";

import {useTheme} from "../../hooks";

import DataTypes, {DataType} from "./DataTypes";
import Functions from "./Functions";
import ObjectTypes from "./ObjectTypes";
import Components from "./Components";

const getBreadcrumbs = (pathname) => {

    const breadcrumbItems = [
        {
            key: "Reference",
            text: "Reference",
            link: "/Documentation/Reference/"
        }
    ];

    if (pathname) {
        const [,,topic] = pathname.split("/");
        if (topic === "Functions") {
            const [,,,category, functionId] = pathname.split("/");

            breadcrumbItems.push({
                key: "Functions",
                text: "Functions",
                link: "/Documentation/Reference/Functions"
            });
     
            if (category)
                breadcrumbItems.push({
                    key: category,
                    text: category,
                    link: "/Documentation/Reference/Functions/" + category
                });

            if (functionId)
                breadcrumbItems.push({
                    key: functionId,
                    text: decodeURIComponent(functionId),
                    link: "/Documentation/Reference/Functions/" + category + "/" + functionId
                });
        } else if (topic === "DataTypes") {
            const [,,,dataType] = pathname.split("/");

            breadcrumbItems.push({
                key: "DataTypes",
                text: "Data Types",
                link: "/Documentation/Reference/DataTypes"
            });

            if (dataType) {
                breadcrumbItems.push({
                    key: dataType,
                    text: dataType,
                    link: "/Documentation/Reference/DataTypes/" + dataType
                });
            }
        } else if (topic === "Schema") {
            const [,,,adaptorId, objectType, objectId] = pathname.split("/");

            breadcrumbItems.push({
                key: "Schema",
                text: "Schema",
                link: "/Documentation/Reference/Schema/"
            });

            if (adaptorId) {
                breadcrumbItems.push({
                    key: adaptorId,
                    text: adaptorId,
                    link: "/Documentation/Reference/Schema/" + adaptorId
                });
            }

            if (objectType) {
                breadcrumbItems.push({
                    key: objectType,
                    text: objectType,
                    link: "/Documentation/Reference/Schema/" + adaptorId + "/" + objectType
                });
            }

            if (objectId) {
                breadcrumbItems.push({
                    key: objectId,
                    text: objectId,
                    link: "/Documentation/Reference/Schema/" + adaptorId + "/" + objectType + "/" + objectId
                });
            }
        } else if (topic === "Components") {
            const [,,,category, component] = pathname.split("/");

            breadcrumbItems.push({
                key: "Components",
                text: "Components",
                link: "/Documentation/Reference/Components/"
            });

            if (category) {
                breadcrumbItems.push({
                    key: category,
                    text: category,
                    link: "/Documentation/Reference/Components/" + category,
                });
            }

            if (component) {
                breadcrumbItems.push({
                    key: component,
                    text: component,
                    link: "/Documentation/Reference/Components/" + category + "/" + component,
                });
            }

        }
    }

    return breadcrumbItems;    

};

export const Reference = () => {
        
    const {functions, error} = useFunctions();
    const {dataTypes} = useDataTypes();
    const {objectTypes: objectTypeObjects} = useObjectTypes();
    const theme = useTheme();

    const {pathname} = useLocation();
    
    const breadcrumbItems = useMemo(() => getBreadcrumbs(pathname), [pathname]);

    useEffect(() => {
        /* if linking to a brand new section, scroll to the top */         
        window.scrollTo(0, 0);
    }, [pathname]);
   

    if (error) {
        return (
            <div>
                <div style={{ height: "10vh" }} />
                <div style={{ textAlign: "center" }}>
                    <Typography color="error" size="7" text="Error Loading Data" />
                    <Typography color="error" text="Reference requires adaptive functions, object type and data type definitions to display." />
                </div>
            </div>
        );
    }
    
    if (!dataTypes || !functions || !objectTypeObjects)
        return <Spinner fullScreen size="large" label="Loading Object Types, Data Types and Functions..." />;

    return (                             
        <Container id="admin-documentation-reference" data-testid="admin-documentation-reference" maxWidth="lg">
            <Breadcrumb items={breadcrumbItems} />
            <div style={{ height: theme.spacing(2) }} />
            <Switch>
                <Route exact path="/Documentation/Reference" render={() => {
                    return (
                        <div style={{ padding: theme.spacing(3) }}>
                            <div>
                                <Typography size="10" text="Reference" />          
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography text="This reference helps you locate specific features provided by the Adaptive Framework core, as well as those provided by extensions and user-defined objects." />
                                <div style={{ height: theme.spacing(5) }} />
                                <Divider />
                                <div style={{ height: theme.spacing(5) }} />
                            </div>
                            <div>                                                                            
                                <Responsive                                         
                                    rows={[
                                        {
                                            spacing: 1,
                                            style: {
                                                paddingBottom: theme.spacing(3)
                                            },                                                                    
                                            columns: [
                                                {                                                                            
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:                                                      
                                                        <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper, minHeight: "300px" }}>
                                                            <div style={{ paddingBottom: theme.spacing(1) }}>
                                                                <Link url="/Documentation/Reference/DataTypes/" text="Data Types"/>
                                                            </div>
                                                            <Typography text="Adaptive Framework provides Data Types, which can be referenced by properties,
                                                                functions, and expressions.  These data types govern how values are represented in various content types 
                                                                and how they are converted from one data type to another." />                                                                
                                                        </div> 
                                                },
                                                {                                                                            
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:                                                      
                                                        <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper, minHeight: "300px" }}>
                                                            <div>                                                   
                                                                <div style={{ paddingBottom: theme.spacing(1) }}>
                                                                    <Link url="/Documentation/Reference/Functions/" text="Functions" />
                                                                </div>                                             
                                                                <Typography text="Adaptive Framework provides Functions, which may be referenced by properties or
                                                                    expressions and evaluated by the Adaptive Framework core to produce values." />                                                                    
                                                            </div>
                                                        </div>     
                                                    
                                                },
                                                {                                                                            
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:                                                       
                                                        <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper, minHeight: "300px" }}>
                                                            <div style={{ paddingBottom: theme.spacing(1) }}>
                                                                <Link url="/Documentation/Reference/Schema/" text="Schema"/>
                                                            </div>
                                                            <Typography text="An Adaptive Schema may be provided by an adaptor, or created through a model.  They
                                                                provide a sets of properties which are used to describe Adaptive Objects.  Object Types may use
                                                                inheritance to factor common properties and provide data validation and computed values." />                                                                
                                                        </div> 
                                                },
                                                {                                                                            
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:                                                       
                                                        <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper, minHeight: "300px" }}>
                                                            <div style={{ paddingBottom: theme.spacing(1) }}>
                                                                <Link url="/Documentation/Reference/Components/" text="Components"/>
                                                            </div>
                                                            <Typography text="Adaptive Components are the building blocks for Adaptive User Interfaces.  They
                                                                provide simple or complex logic for transforming data and rendering a UI to allow the user
                                                                to interact with data." />                                                                
                                                        </div> 
                                                }
                                            ]
                                            
                                        }
                                    ]}
                                />
                            </div>
                        </div>
                    );
                }} />                             
                
                <Route exact path="/Documentation/Reference/DataTypes" component={DataTypes} />                        
                <Route path="/Documentation/Reference/DataTypes/:dataTypeId" component={DataType} />                        
                <Route exact path="/Documentation/Reference/Schema" component={ObjectTypes} />
                <Route exact path="/Documentation/Reference/Schema/:adaptorId" component={ObjectTypes} />
                <Route exact path="/Documentation/Reference/Schema/:adaptorId/:objectType" component={ObjectTypes} />
                <Route exact path="/Documentation/Reference/Schema/:adaptorId/:objectType/:propertyName" component={ObjectTypes} />
                <Route path="/Documentation/Reference/Components" component={Components} />
                <Route path="/Documentation/Reference/Functions" component={Functions} />
            </Switch>
        </Container>
    );
};

export default Reference;
