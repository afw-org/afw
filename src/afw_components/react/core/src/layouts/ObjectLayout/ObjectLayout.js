// See the 'COPYING' file in the project root for licensing information.

import {useOperational, useAfwObject} from "../../hooks";
import {layoutByObject} from "../registry";

/**
 * ObjectLayout
 * 
 * This renders a layout for a given object and operational context, from 
 * which it reads from React context.  It uses the registry to determine
 * the appropriate layout to use.
 */
export const ObjectLayout = (props) => {

    const object = useAfwObject(props);
    const operational = useOperational(props);

    const Layout = layoutByObject({ object, operational });

    if (Layout) {
        return <Layout {...props} />;
    } else return null;
};

export default ObjectLayout;
