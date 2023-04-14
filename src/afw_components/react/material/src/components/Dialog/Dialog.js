// See the 'COPYING' file in the project root for licensing information.

import MuiDialog from "@mui/material/Dialog";
import MuiDialogTitle from "@mui/material/DialogTitle";
import MuiDialogContent from "@mui/material/DialogContent";
import MuiDialogActions from "@mui/material/DialogActions";
import MuiDialogContentText from "@mui/material/DialogContentText";
import Icon from "@mui/material/Icon";
import IconButton from "@mui/material/IconButton";

import {useTheme} from "@mui/material/styles";
import useMediaQuery from "@mui/material/useMediaQuery";

import {AdaptiveComponent} from "@afw/react";

export const Dialog = (props) => {
    const theme = useTheme();
    const small = useMediaQuery(theme.breakpoints.down("sm"));
    const fullScreen = small && (["lg","xl","xxl","xxxl"].includes(props.maxWidth));

    return (
        <MuiDialog                    
            open={props.open ? true : false}
            onClose={props.isBlocking ? () => undefined : props.onDismiss}   
            fullScreen={props.fullScreen || fullScreen}  
            maxWidth={props.maxWidth}          
            fullWidth={true}     
            data-testid={props["data-testid"]}
        >
            {
                props.showClose && (                                       
                    <IconButton    
                        style={{ position: "absolute", right: "8px", top: "8px" }}
                        onClick={props.onDismiss}
                        aria-label="Close Dialog"
                    >
                        <Icon>close</Icon>
                    </IconButton>                            
                )
            }
            {
                props.title &&                
                <MuiDialogTitle>{props.title}</MuiDialogTitle>
            }
            <MuiDialogContent>
                <MuiDialogContentText>{props.subText}</MuiDialogContentText>
                <AdaptiveComponent layoutComponent={props.contains} />
            </MuiDialogContent>
            <MuiDialogActions>
                <AdaptiveComponent layoutComponent={props.footer} />
            </MuiDialogActions>
        </MuiDialog>
    );
};

export default Dialog;
