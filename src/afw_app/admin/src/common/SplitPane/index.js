// See the 'COPYING' file in the project root for licensing information.
// This was taken from:  https://github.com/tomkp/react-split-pane
// It was an MIT-licensed project, which never got updated and caused all sorts of 
// package dependency issues because of this.

import {useEffect, forwardRef} from "react";
import {useTheme} from "../../hooks";

import ReactSplitPane from "./SplitPane";
import Pane from "./Pane";

export const SplitPane = forwardRef((props, ref) => {

    const theme = useTheme();

    useEffect(() => {
        if (theme.palette.mode === "dark")
            import("./Resizer.dark.css");
        else 
            import("./Resizer.light.css");
    }, [theme]);


    return (
        <ReactSplitPane {...props} ref={ref} />
    );
});

export default SplitPane;
export { Pane };
