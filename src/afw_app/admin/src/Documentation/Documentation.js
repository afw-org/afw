// See the 'COPYING' file in the project root for licensing information.
import {Outlet} from "@tanstack/react-router";

import Container from "../common/Container";

import {
    Divider,   
    Typography
} from "@afw/react";

import {useTheme} from "../hooks";


/*
 * DocumentationHome
 *
 * The /Documentation index (see routes.js).
 */
export const DocumentationHome = () => {

    const theme = useTheme();

    return (
        <>
            <Typography size="10" text="Documentation" />
            <div style={{ height: theme.spacing(5) }} />
            <Typography text="The Documentation provides Reference material for the objects that are available in the currently running instance of Adaptive Framework." />
            <div style={{ height: theme.spacing(5) }} />
            <Divider />                            
        </>
    );
};

/*
 * Documentation
 *
 * The /Documentation layout route's component (see routes.js).
 */
const Documentation = () =>
    <Container style={{ height: "100%" }}>
        <Outlet />
    </Container>;

export default Documentation;
