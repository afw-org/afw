// See the 'COPYING' file in the project root for licensing information.
import {memo} from "react";
import MuiToolbar from "@mui/material/Toolbar";
import Grid from "@mui/material/Grid";

import {AdaptiveComponent} from "@afw/react";

export const Toolbar = (props) => {
    return (
        <MuiToolbar 
            id={props.id}
            data-testid={props["data-testid"]}
            data-component-type={props["data-component-type"]}
            className={props.className}
            style={props.style}
            disableGutters={true}
            variant={props.dense ? "dense" : "regular"}                    
        >
            <Grid
                container
                spacing={props.spacing}
                sx={{
                    alignItems: props.alignItems,
                    justifyContent: props.justify
                }}>
                {
                    props.contains && props.contains.map((item, index) =>
                        item ?
                            <Grid key={index} size={props.width === "auto" ? true : false}>
                                <AdaptiveComponent layoutComponent={item} />
                            </Grid>
                            :
                            <AdaptiveComponent layoutComponent={item} />
                    )
                }
            </Grid>
        </MuiToolbar>
    );   
};

export default memo(Toolbar);
