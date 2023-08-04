// See the 'COPYING' file in the project root for licensing information.
import {ObjectResponsive} from "@afw/react";

import {useTheme} from "../../hooks";

const RequestHandlerDetails = (props) => {
    
    const theme = useTheme();
    const requestHandler = props.requestHandler;
    
    if (!requestHandler)
        return null;
    
    return (
        <ObjectResponsive 
            object={requestHandler}
            spacing={theme.spacing(1)}
        />
    );
};

export default RequestHandlerDetails;
