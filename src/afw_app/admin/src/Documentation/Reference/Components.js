// See the 'COPYING' file in the project root for licensing information.
import {useParams} from "@tanstack/react-router";

import {
    Link,
    Message,
    Spinner,
    Table,
    Typography,
    useLayoutComponentTypes,  
    useLayoutComponentTypeCategories,      
} from "@afw/react";
 
import {useTheme} from "../../hooks";

const Component = () => {

    const {layoutComponentTypes} = useLayoutComponentTypes();
    const theme = useTheme();
    const {component} = useParams({ strict: false });

    if (!layoutComponentTypes)
        return <Spinner size="large" label="Loading Component Types..." fullScreen={true} />;

    let matchingComponent = layoutComponentTypes.filter(type => type.componentType === component);
    
    if (matchingComponent.length === 0)
        return <Typography text={"Could not find component " + component} />;

    const theComponent = matchingComponent[0];

    return (
        <div>
            <Typography size="8" text={component} />
            <div style={{ height: theme.spacing(3) }} />

            <Typography text={theComponent.description} />
            <div style={{ height: theme.spacing(3) }} />

            <Typography size="6" color="primary" text="Category" />
            <div style={{ height: theme.spacing(1) }} />
            <Typography text={theComponent.category} />
            <div style={{ height: theme.spacing(3) }} />

            <Typography size="6" color="primary" text="Parameters" />
            <div style={{ height: theme.spacing(3) }} />

            <Typography size="6" color="primary" text="Demo" />
            <div style={{ height: theme.spacing(3) }} />
        </div>
    );
};

const ComponentCategory = () => {

    const {layoutComponentTypes} = useLayoutComponentTypes();
    const {category} = useParams({ strict: false });
    
    if (!layoutComponentTypes)
        return <Spinner size="large" label="Loading Component Types..." fullScreen={true} />;

    const matchingComponents = layoutComponentTypes.filter(type => type.category === category);    

    return (        
        <Table 
            columns={[
                {
                    key: "Component",
                    name: "Component",
                    onRender: (component) => 
                        <Link 
                            url={"/Documentation/Reference/Components/" + category + "/" + encodeURIComponent(component.label)}
                            text={component.label}
                        />
                },
                {
                    key: "Description",
                    name: "Description",
                    onRender: (component) => 
                        <Typography text={component.brief} />
                }
            ]}
            rows={matchingComponents}
            selectionMode="none"
            compact={true}
        />                          
    );
};

const ComponentCategories = () => {

    const {layoutComponentTypeCategories} = useLayoutComponentTypeCategories();
    
    if (!layoutComponentTypeCategories)
        return <Spinner size="large" label="Loading Component Categories..." fullScreen={true} />;

    return (
        <>
            <Message 
                message="Adaptive Framework provides UI Components, or building blocks for constructing Adaptive Layouts.
                Adaptive Layouts are used to render UI interfaces for objects and properties."
            />
            <Table 
                columns={[
                    {
                        key: "Category",
                        name: "Category",
                        onRender: (category) => 
                            <Link 
                                url={"/Documentation/Reference/Components/" + encodeURIComponent(category.category)}
                                text={category.label}
                            />
                    },
                    {
                        key: "Description",
                        name: "Description",
                        onRender: (category) => 
                            <Typography text={category.description} />
                    }
                ]}
                rows={layoutComponentTypeCategories}
                selectionMode="none"
                compact={true}
            />    
        </>
    );
};

export const Components = () => {   
    
    const theme = useTheme();
    /* the route's optional params (see ../routes.js) */
    const {category, component} = useParams({ strict: false });

    /* one component, a category's components, or the categories */
    if (component)
        return (
            <div>                    
                <Component />
            </div>
        );

    if (category)
        return (
            <div>
                <Typography size="8" text="Components" />
                <div style={{ height: theme.spacing(3) }} />
                <ComponentCategory />
            </div>
        );

    return (
        <div>
            <Typography size="8" text="Component Categories" />
            <div style={{ height: theme.spacing(3) }} />
            <ComponentCategories />
        </div>
    );
};

export default Components;
