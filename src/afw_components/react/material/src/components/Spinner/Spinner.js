// See the 'COPYING' file in the project root for licensing information.
import {memo} from "react";
import CircularProgress from "@mui/material/CircularProgress";
import Typography from "@mui/material/Typography";
import Button from "@mui/material/Button";
import Backdrop from "@mui/material/Backdrop";
import Grow from "@mui/material/Grow";

import {useClasses, useTheme} from "@afw/react";

const styles = theme => ({
    circularProgress: {
        display: "block",
        margin: "0 auto"
    },
    text: {
        textAlign: "center",
        padding: theme.spacing(1),
        color: theme.palette.text.primary
    },
    fullScreen: {
        position: "absolute",      
        minWidth: "200px",  
        left: "50%",
        top: "50%",
        transform: "translate(-50%,-50%)",
        backgroundColor: theme.palette.background.paper,
        padding: theme.spacing(2),
        borderRadius: theme.shape.borderRadius,
    }, 
    fullScreenBackdrop: {
        backgroundColor: theme.palette.background.paper,
        padding: theme.spacing(2),
        borderRadius: theme.shape.borderRadius,
        minWidth: "200px",
        position: "absolute",
        transform: "translate(-50%, -50%)"
    },
    center: {
        width: "100%",
        margin: "0 auto"
    },
    button: {
        display: "block", 
        margin: "0 auto" 
    },
    backdrop: {
        zIndex: theme.zIndex.drawer + 1,
        color: "#fff"
    }
});

export const Spinner = ({ size, className, style, label, fullScreen, onCancel, backdrop, ...rest }) => {
    let variant;
    const classes = useClasses(styles);
    const theme = useTheme();

    if (size === "small") {
        size = 20;
        variant = "body2";
    } else if (size === "medium") {
        size = 40;
        variant = "body1";
    } else if (size === "large") {
        size = 80;
        variant = "h6";
    } else {
        size = 40;
        variant = "body1";
    }

    const spinner = (
        <>
            <CircularProgress
                className={className + " " + classes.circularProgress}
                style={{ ...style }}
                size={size}
                {...rest}
            />
            <Typography
                className={classes.text}
                variant={variant}
            >{label}</Typography>
            {
                onCancel && (
                    <>
                        <div style={{ height: theme.spacing(1) }} />
                        <Button 
                            className={classes.button}
                            color="primary" 
                            variant="outlined"
                            onClick={onCancel} 
                        >
                            Cancel
                        </Button>
                    </>
                )
            }
        </>
    );

    if (fullScreen) {
        if (backdrop)
            return (
                <Backdrop open={true} className={classes.backdrop}>
                    <Grow in={true} timeout={1000}>
                        <div className={classes.fullScreenBackdrop}>
                            { spinner }
                        </div>
                    </Grow>
                </Backdrop>
            );
        else 
            return (       
                <div style={{ position: "fixed", top: 0, left: 0, right: 0, bottom: 0 }}>
                    <div className={classes.fullScreen}>
                        { spinner }
                    </div>                                
                </div>
            );
    }

    return (
        <div className={classes.center}>
            { spinner }
        </div>
    );   
};

export default memo(Spinner);
