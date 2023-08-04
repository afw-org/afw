// See the 'COPYING' file in the project root for licensing information.
import {createContext} from "react";

/**
 * AFW React Core Context
 *
 * This file provides React context that can be used by
 * applications to setup AFW React Core providers and
 * consumers of context data.
 */


/**
 * The ComponentsContext allows applications to setup the provider for
 * Adaptive Layout Components.
 */
export const ComponentsContext = createContext({ components: [] });

/**
 * The ModelContext allows applications to setup the provider for
 * an AfwModel.
 */
export const ModelContext = createContext();


/**
 * The QualifiersContext allow for components to decide which
 * qualifiers are available for adaptive expressions within its
 * component tree.
 */
export const QualifiersContext = createContext();

/**
 * The OperationalContext allows for components to know if
 * they are working with an editable object or property.
 * This may influence the way in which their content is
 * rendered.
 */
export const OperationalContext = createContext({});
export const OperationalMode = {
    Editable:       { editable: true    },
    NotEditable:    { editable: false   },
    Add:            { add: true         },
    NotAdd:         { add: false        },
};

/**
 * The LayoutContext provides layoutParameters for
 * components rendered within the AdaptiveLayout component.
 */
export const AdaptiveLayoutContext = createContext({ data: {}, layoutParameters: {} });

/**
 * When an application uses Adaptive Components that need to create
 * Links, we use this context to get the base path from in order to
 * navigate within the application properly.
 */
export const RouteBasePathContext = createContext();


/**
 * A context for using Clipboard copy functionality.
 */
export const ClipboardContext = createContext();

/**
 * A context for editing an object.  Can provide appropriate functions 
 * and parameters to allow for property rendering, header rendering, context 
 * menus, save functions, copy/paste, etc.
 */
export const ObjectEditorContext = createContext({});
