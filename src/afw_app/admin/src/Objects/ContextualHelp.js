// See the 'COPYING' file in the project root for licensing information.
import {
    Divider,
    Icon,
    Typography
} from "@afw/react";

import {useTheme} from "../hooks";

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

/**
 * ContextualHelp components for /Objects route.
 */
export const ObjectEditorHelp = ({ /*selectedAdapterId, selectedObjectTypeId,*/ selectedObject }) => {

    const theme = useTheme();
    const properties = (selectedObject ? selectedObject.getProperties().sort((A, B) => A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())) : []);

    return (
        <>
            <HelpText text={` 
                The Object Editor allows you to view an individual Adaptive Object in detail, including \
                it's Properties and Metadata.

                Use the toolbar's menu button to bring up a list of actions you may perform on the \
                object.            

            `} />
            <div style={{ textAlign: "center" }}>
                <Icon iconName="more_vert" />
            </div>
            <ul>
                <li><Typography size="3" text="Select Edit to unlock the Object for editing." /></li>
                <li><Typography size="3" text="Select Copy to copy a property from this object." /></li>
                <li><Typography size="3" text="Select Paste to paste in a property for this object from another object." /></li>
                <li><Typography size="3" text="Select Save to save changes made to this object." /></li>
                <li><Typography size="3" text="Select Replace to save changes made to this object by replacing all of its contents." /></li>
                <li><Typography size="3" text="Select Review Changes to examine changes made to this object." /></li>
                <li><Typography size="3" text="Select Discard Changes to undo any changes currently made to this object." /></li>
                <li><Typography size="3" text="Select Refresh to fetch a new copy of the object." /></li>
            </ul>                
            <HelpText text={` 
                Use the toolbar's toggle buttons to switch between different layouts, in order to view \
                the object from different perspectives.  From left to right, these layouts are: \
                Responsive, Flattened, Tree and Source.       

            `} />
            <div style={{ textAlign: "center" }}>
                <Icon style={{ display: "inline-block", marginRight: theme.spacing(1) }} iconName="view_quilt" />
                <Icon style={{ display: "inline-block", marginRight: theme.spacing(1) }}  iconName="format_list_bulleted" />
                <Icon style={{ display: "inline-block", marginRight: theme.spacing(1) }}  iconName="share" />
                <Icon style={{ display: "inline-block" }}  iconName="code" />
            </div>
            <HelpText text={`
                Click the "Unlock to make changes" icon to make the object editable.  Some objects will  \
                not be editable, making the icon disabled.  Once the object is editable, you may make \
                changes to the object, using any of the appropriate layouts.  When you are finished, \
                you may click "Cancel" to discard changes, or "Save" to save the changes back out to \
                the adapter.               
            `} />
            <div style={{ textAlign: "center" }}>
                <Icon style={{ display: "inline-block" }} iconName="lock" />
            </div>
            <Divider style={{ margin: theme.spacing(1) }} />
            {
                properties.map((property, index) => 
                    <div key={index}>
                        { property.getLabel() ? 
                            <Typography color="primary" text={property.getLabel() + " (" + property.getName() + ")"} /> :
                            <Typography color="primary" text={property.getName()} /> 
                        }
                        <HelpText text={property.getDescription()} />
                        <Typography size="3" text={"This property has a dataType of " + property.getDataType() + "."} />
                        {
                            (property.getRequired() === true) ? <Typography size="3" text="This property is required." /> : null
                        }
                        {
                            (property.getAllowQuery() === true) ? <Typography size="3" text="This property can be queried." /> : null
                        }
                        {
                            (property.getAllowWrite() === false) ? <Typography size="3" text="This property cannot be changed." /> : null
                        }
                        <Divider style={{ margin: theme.spacing(1) }} />
                    </div>
                )
            }
        </>
    );
};

export const ObjectsTableHelp = (/*{ selectedAdapterId, selectedObjectTypeId }*/) => {

    return (
        <>
            <HelpText text={` \
                The Objects Table displays a list of objects, returned by a prior Retrieve operation.  Each row \
                represents an individual object, which you may click on to view details.      
                
                The Toolbar at the top provides general operations that may be performed on the Object(s) in this \
                table.  
            `} />
        </>
    );
};

export const ObjectsHelp = (/*{ selectedAdapterId, selectedObjectTypeId }*/) => {
    return (
        <>
            <HelpText text={` \
                The Objects page allows you to search or create new Objects for a specified adapter.

                First, select the adapter from the Dropdown component labeled "Adapter".  Next, type or select \
                the Object Type to work with from the Dropdown component labeled "Object Type".

            `} />

            <Typography color="primary" size="5" text="Retrieving Objects" />
            <HelpText text={`
                If you wish to Retrieve all Objects of this Object Type, click the "Retrieve" button.

                If you have criteria to search on, enter the filter in the Text Field labeled "query Criteria" in the \
                format "property=value".  You may also create more complex filters by joining them with & and | operators. \
                Click the button labeled "Retrieve" to return the objects to search for.

            `} />

            <Typography color="primary" size="5" text="Getting an Object" />
            <HelpText text={`
                The get_object() operation is often quicker for an adapter to perform, as it may not require indexes or scans to \
                return the object in question.

                If you want to Get a specific object, and you know the objectId, first un-check the "Retrieve" Checkbox.  Then \
                enter an objectId into the Text Field labeled "Object Id".  Finally, click "Get" to lookup the object.

            `} />

            <Typography color="primary" size="5" text="Create New Object" />
            <HelpText text={`
                If you want to create a new Object for this Adapter and Object Type, click the button labeled "New Object".
            `} />
        </>
    );
};


export const ContextualHelpRoutes = [
    { path: "/Objects/:adapterId/:objectTypeId/:objectId",  title: "Objects Editor Help",   Component: ObjectEditorHelp    },
    { path: "/Objects/:adapterId/:objectTypeId",            title: "Objects Table Help",    Component: ObjectsTableHelp    },
    { path: "/Objects/",                                    title: "Objects Help",          Component: ObjectsHelp         },
];

export default ContextualHelpRoutes;
