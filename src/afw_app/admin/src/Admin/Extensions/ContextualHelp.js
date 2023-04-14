// See the 'COPYING' file in the project root for licensing information.
import { 
    Divider,
    Typography
} from "@afw/react";

import {useTheme} from "../../hooks";

const HelpText = ({ text }) => {
    return (
        <Typography 
            size="3"
            style={{ whiteSpace: "pre-line" }}
            paragraph={true}
            text={text.replaceAll("\\n", "\n")}
        />
    );
};


export const ExtensionsHelp = ({ manifests }) => {

    const theme = useTheme();
    const sortedManifests = manifests ? manifests.slice().sort((A, B) => 
        A.getPropertyValue("extensionId").toLowerCase().localeCompare(B.getPropertyValue("extensionId"))
    ) : [];

    return (
        <>
            <HelpText text={` \
                Extensions are compiled shared libraries that, when registered and loaded with Adaptive Framework Core, provide \
                data types, functions, content types, adaptors, request handlers, and more.  They are intended to extend \
                the functionality of Adaptive Framework core using a loadable module interface.

                This page displays all extensions that are known by Adaptive Framework Core and represented in a table \
                format.  Extensions that have been loaded will show version information, while extensions that have not \
                been loaded will remain unknown until they are.

                Extensions that are currently loaded cannot be unloaded without stopping the server and manually removing \
                them from startup configuration.

                To load an extension, select the extension and click "Load Extension".  You will be presented with a dialog \
                to confirm this decision, with an optional checkbox to allow you to automatically load it on Application Startup.
            `} />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />

            {
                sortedManifests.map(manifest => (
                    <div key={manifest.getPropertyValue("extensionId")}>
                        <div style={{ height: theme.spacing(2) }} />
                        <Typography color="primary" size="4" text={manifest.getPropertyValue("extensionId")} />
                        <HelpText text={manifest.getPropertyValue("description")} />
                    </div>
                ))
            }

        </>
    );
};


export const ContextualHelpRoutes = [

    { path: "/Admin/Extensions",    title: "Extension Help",    Component: ExtensionsHelp   },    

];

export default ContextualHelpRoutes;
