// See the 'COPYING' file in the project root for licensing information.
import {Typography} from "@afw/react";

import {useTheme} from "../hooks";

export const NoRoute = () => {
    const theme = useTheme();

    return (
        <div style={{ padding: theme.spacing(3) }}>
            <Typography size="7" text="Invalid Route" />
            <div style={{ height: theme.spacing(5) }} />
            <Typography text="Please check the url for mistakes." />
        </div>
    );
};

export default NoRoute;
