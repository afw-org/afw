// See the 'COPYING' file in the project root for licensing information.
import {
    Typography
} from "@afw/react";

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


export const ModelsHelp = () => {
    return (
        <>
            <HelpText text={` \
                Adaptive Models allow you to define Object Types and Property 
                Types that will be used to map or transform values from other 
                adapters and even run scripts when objects are accessed by the 
                model.

                Select a model from the list, or create a new model.
            `} />
        </>
    );
};

export const ModelEditorHelp = () => {
    return (
        <>
            <HelpText text={` \
                When viewing or editing a model, there are a variety of 
                perspectives you can choose from to use.
            `} />
        </>
    );
};

export const ModelEditorObjectTypesHelp = () => {
    return (
        <>
            <HelpText text={` \
                The Model uses Object Types to describe objects that will be 
                accessed or managed through this Model.

                Within an Object Type, you can define new Property Types, or 
                read and edit an existing one. To begin, click the lock icon 
                next to "Edit Model", to enter edit mode.
            `} />
        </>
    );
};

export const ModelEditorPropertyTypesHelp = () => {
    return (
        <>
            <HelpText text={` \
                The Model uses Property Types to describe properties that will 
                be accessed or managed through this Model Object Type.
            `} />
        </>
    );
};

export const ContextualHelpRoutes = [

    { 
        path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectTypeId/propertyTypes/:propertyType",    
        title: "Model Editor Help", 
        Component: ModelEditorPropertyTypesHelp  
    },
    { 
        path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectTypeId",    
        title: "Model Editor Help", 
        Component: ModelEditorObjectTypesHelp  
    },
    { 
        path: "/Admin/Models/:adapterId/:modelId",    
        title: "Model Editor Help", 
        Component: ModelEditorHelp  
    },
    { 
        path: "/Admin/Models",                        
        title: "Models Help",       
        Component: ModelsHelp       
    },

];

export default ContextualHelpRoutes;
