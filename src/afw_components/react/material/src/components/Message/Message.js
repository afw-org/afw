// See the 'COPYING' file in the project root for licensing information.
import Typography from "@mui/material/Typography";
import Icon from "@mui/material/Icon";
import Alert from "@mui/material/Alert";

export const Message = ({ canDismiss, onDismiss, status = "info", contains, message }) => {

    return (
        <Alert
            severity={status}
            action={
                (canDismiss || onDismiss) ? 
                    <Icon onClick={onDismiss}>close</Icon> : undefined
            }
        >
            {
                contains ? contains : <Typography>{message}</Typography>
            }
        </Alert>
    );
};


export default Message;
