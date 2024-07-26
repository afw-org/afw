// See the 'COPYING' file in the project root for licensing information.
import {AdaptiveLayoutContext} from "@afw/react";
import {useAdaptiveLayout} from "@afw/react";


/**
 * AdaptiveLayout 
 *
 * An AdaptiveLayout takes 5 primary inputs:
 * 
 *     1.  Imperative Layout Instructions
 * 
 *         Specific components to be inserted where and how.  These instructions may augment
 *         data to provide additional UI inputs and controls, or completely replace data and
 *         simply tell the AdaptiveLayout how to apply the components that are instructed.
 * 
 *         This is represented by the prop, "layout".  This props may be a React element, 
 *         called programmatically by an Application, or an object representing components.
 * 
 *         layout={
 *             <div>
 *                 <Typography ... />
 *                 <Tabs ... />
 *                 <TextField ... />
 *             </div>
 *         } 
 * 
 *     2.  Data
 * 
 *         Objects, Properties, Values, Lists can all be supplied here.  The data may be JSON
 *         data or Adaptive instances (AfwObject, AfwProperty, etc.).  When using 
 *         Adaptive instances, the data state is managed through the class methods.  When using
 *         JSON objects, metadata must be supplied manually.  The data may also be a URI, which
 *         the AdaptiveLayout will use to fetch, where necessary.
 * 
 *         data={
 *             object: <AfwObject> | <JSON>,
 *             objectUri: "/adapterId/objectType/objectId",
 *             property: <AdaptiveProperty>,
 *             objectType: <JSON>,
 *         }
 * 
 *     3.  Layout Parameters
 * 
 *         Determines how to manage the data.  This includes Sorting, Filtering, Include/Exclude, 
 *         Labeling, Descriptions, etc.  These may be specific to components that are nested within
 *         the layout.
 * 
 *     4.  Metadata Built-ins
 * 
 *         Default handlers for object types, property types and data types.  These are compiled 
 *         in as Javascript components.  For example, dataType=string will be handled by TextFields 
 *         and dataType=boolean will be handled by checkboxes, by default.  Some default propertyTypes
 *         also have defaults, such as adapterId and dataType dropdown pickers.
 * 
 *         String => TextField
 *         Boolean => Checkbox
 *         adapterId => AdapterDropdown
 *         propertyType => PropertyTypeDropdown
 * 
 *     5.  Registered Built-ins
 * 
 *         Apps and extensions can have registered handlers for object types, property types and data types.  
 *         These are similar to the Metadata built-ins, but defined at the Application level.
 * 
 *         ObjectResponsive
 *         ObjectTree
 *         ObjectSource
 * 
 * The output of an Adaptive Layout is a composition of Adaptive Components, derived from these inputs.
 * 
 */


/* a simple intermediate component to merge layout providers, so they can be nested and props can be merged together */
export const AdaptiveLayoutMergeProviders = ({ layout, layoutParameters, data, children }) => {

    const ctxLayout = useAdaptiveLayout();

    let mergedLayout = {};    
    mergedLayout.layout = { ...ctxLayout.layout, ...layout };      
    mergedLayout.layoutParameters = { ...ctxLayout.layoutParameters, ...layoutParameters };
    mergedLayout.data = { ...ctxLayout.data, ...data};    

    return (
        <AdaptiveLayoutContext.Provider value={mergedLayout}>
            { children }
        </AdaptiveLayoutContext.Provider>
    );
};
