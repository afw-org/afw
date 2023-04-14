// See the 'COPYING' file in the project root for licensing information.
import {ObjectResponsive} from "../ObjectResponsive";

export const AdaptiveFunction = (props) => {

    return (
        <ObjectResponsive 
            {...props}
            embedded={{
                component: "Box"
            }}                     
        />
    );
};

export default AdaptiveFunction;
