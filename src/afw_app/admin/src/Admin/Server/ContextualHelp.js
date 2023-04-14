// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";

import { AfwObject } from "@afw/client";
import {
    Divider,
    Typography,
    useServer,
    useSystemInfo,
    useModel,
} from "@afw/react";

import {useTheme} from "../../hooks";

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

const TabHelp = ({ tab, conf }) => {

    const theme = useTheme();
    const [afwServerConf, setAfwServerConf] = useState();
    const model = useModel();

    useEffect(() => {
        const initialize = async () => {
            const afwServerConf = new AfwObject({ model, object: conf, objectTypeId: "_AdaptiveServer_" });
            await afwServerConf.initialize();

            setAfwServerConf(afwServerConf);
        };

        if (conf) {
            initialize();
        }
    }, [model, conf]);
    
    if (!afwServerConf)
        return null;

    const properties = afwServerConf.getProperties().sort((A, B) => {

        const a = A.getLabel() ? A.getLabel() : A.getName();
        const b = B.getLabel() ? B.getLabel() : B.getName();

        return a.toLowerCase().localeCompare(b.toLowerCase());
    });

    return (
        <>
            <Typography color="primary" size="6" text={tab} />
            {                
                properties.map((property, index) => (
                    <div key={index}>
                        <div style={{ height: theme.spacing(2) }} />
                        <Typography color="primary" size="4" text={property.getLabel()} />
                        <HelpText text={property.getDescription()} />
                    </div>
                ))
            }            
        </>
    );
};


export const ServerHelp = () => {

    const theme = useTheme();
    const {server} = useServer();
    const {systemInfo} = useSystemInfo();

    return (
        <>
            <HelpText text={` \
                This page presents you with some information about the instance of your Adaptive Server, along with System Information describing its environment.    
                
                The Adaptive Server is a compiled application, which is capable of processing client requests.  The server is created from \
                from command-line parameters, which may define read-only properties such as the number of requests it can handle concurrently and the number of  \
                requests and errors it is or has currently processed, the semantic versions it was compiled with and against, and the time it was started.

                The System Information describes the underlying environment or Operating System, including its architecture and resource settings.

                All of this information may be useful in troubleshooting server-side problems.
            `} />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />
            <div style={{ height: theme.spacing(1) }} />

            <TabHelp tab="Server Information" conf={server} />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />
            <div style={{ height: theme.spacing(1) }} />
            <Typography color="primary" size="6" text="System Information" />
            <div style={{ height: theme.spacing(2) }} />

            {
                systemInfo && systemInfo.filter(sysInfo => sysInfo._meta_.objectId !== "initialEnvironmentVariables").map(sysInfo => {
                    return (
                        <div key={sysInfo._meta_.objectId}>                            
                            <TabHelp tab={sysInfo._meta_.objectI} conf={sysInfo} />
                            <div style={{ height: theme.spacing(1) }} />
                            <Divider />
                            <div style={{ height: theme.spacing(1) }} />
                        </div>                        
                    );
                })
            }

        </>
    );
};



export const ContextualHelpRoutes = [

    { path: "/Admin/Server",    title: "Server Help",   Component: ServerHelp   },

];

export default ContextualHelpRoutes;
