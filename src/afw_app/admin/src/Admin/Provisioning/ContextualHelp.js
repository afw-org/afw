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

export const ProvisioningHelp = () => {
    return (
        <>            
            <HelpText text={` \
                Provisioning involves the process of transferring data between peers \
                using a configured set of rules for which data should be transferred and \
                how the data should be transformed.                
            `} />
        </>
    );
};



export const ContextualHelpRoutes = [

    { path: "/Admin/Provisioning",      title: "Provisioning Help",     Component: ProvisioningHelp   },

];

export default ContextualHelpRoutes;
