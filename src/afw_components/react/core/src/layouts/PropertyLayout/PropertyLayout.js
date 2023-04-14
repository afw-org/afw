// See the 'COPYING' file in the project root for licensing information.

import {useOperational, useAfwProperty} from "../../hooks";
import {layoutByProperty} from "../registry";

/**
 * PropertyLayout
 * 
 * This renders a layout for a given property and operational context, from 
 * which it reads from React context.  It uses the registry to determine
 * the appropriate layout to use.
 */
export const PropertyLayout = (props) => {
    
    const property = useAfwProperty(props);
    const operational = useOperational(props);

    const Layout = layoutByProperty({ property, operational });

    if (Layout) {
        return <Layout {...props} />;
    } else return null;
};

export default PropertyLayout;
