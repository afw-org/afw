// See the 'COPYING' file in the project root for licensing information.
import { ObjectResponsive } from "@afw/react";

export const PropertyType = ({ propertyTypeObject }) => {
    
    return (
        <ObjectResponsive 
            object={propertyTypeObject}
            filterOptions={{
                filterValuedProperties: true
            }}
        />
    );
};

export default PropertyType;
