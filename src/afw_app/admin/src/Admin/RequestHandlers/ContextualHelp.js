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


export const RequestHandlersHelp = () => {
    return (
        <>
            <HelpText text={` \
                
                
            `} />
        </>
    );
};


export const ContextualHelpRoutes = [

    { path: "/Admin/RequestHandlers",   title: "Request Handlers Help", Component: RequestHandlersHelp   },

];

export default ContextualHelpRoutes;
