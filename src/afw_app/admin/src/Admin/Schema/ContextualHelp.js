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


const SchemaHelp = () => {
    return (
        <>
            <HelpText text={` \
                An Adaptive Schema is a set of Object Types and Property Types that are produced by an adaptor.

                For some adaptors, the Adaptive Schema may be read-only, or only editable by using external tools, \
                specifically designed for that specific adaptor.  For other adaptors, you may be able to manage the \
                Adaptive Schema by saving the Object Types and Property Types directly into the adaptor store.

                Choose an adaptor from the table by clicking on its link.
            `} />
        </>
    );
};

const SchemaAdaptorHelp = () => {

    return (
        <>
            <HelpText text={` \
                If this adaptor allows for the Adaptive Schema to be managed here, you will be presented with \
                a table of Object Types.  Select an Object Type to edit, or click the "New" or "Remove" buttons \
                to add a new Object Type or remove an existing one.
            `} />
        </>
    );
};

const SchemaObjectTypeHelp = () => {

    return (
        <>
            <HelpText text={` \
                
            `} />
        </>
    );
};

const SchemaPropertyTypeHelp = () => {

    return (
        <>
            <HelpText text={` \
                
            `} />
        </>
    );
};

export const ContextualHelpRoutes = [

    { path: "/Admin/schema/:adaptorId/:objectTypeId/:propertyType", title: "Schema Editor Help",    Component: SchemaPropertyTypeHelp     },
    { path: "/Admin/schema/:adaptorId/:objectTypeId",               title: "Schema Editor Help",    Component: SchemaObjectTypeHelp     },
    { path: "/Admin/schema/:adaptorId",                             title: "Schema Editor Help",    Component: SchemaAdaptorHelp        },
    { path: "/Admin/Schema",                                        title: "Schema Help",           Component: SchemaHelp               },

];

export default ContextualHelpRoutes;
