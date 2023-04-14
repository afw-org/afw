// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Dialog,
    Typography
} from "@afw/react";

import {useTheme} from "../hooks";

export const AppError = (props) => {    
    
    const theme = useTheme();
    const {title, message, error, info, reload} = props;

    return (
        <Dialog 
            title={title}
            open={true}
            maxWidth="lg"
            subText={message}
            contains={
                <div style={{ maxHeight: "70vh", overflow: "auto", padding: theme.spacing(2) }}>                                        
                    {
                        (typeof(error) === "string") &&
                            <>                                
                                <Typography style={{ fontWeight: "bold", marginRight: theme.spacing(0.5) }} color="error" text="Error Message: " />
                                <Typography style={{
                                    fontFamily: "monospace",
                                    whiteSpace: "pre-wrap"
                                }} text={error} color="error" />
                            </>
                    }
                    {
                        (error && error.stack) &&
                            <>
                                <div style={{ height: theme.spacing(2) }} />                                
                                <Typography style={{ fontWeight: "bold", marginRight: theme.spacing(0.5) }} color="error" text="Error Stack: " />
                                <Typography style={{
                                    fontFamily: "monospace",
                                    whiteSpace: "pre-wrap",
                                }} text={error.stack} color="error" />
                            </> 
                    }
                    {
                        (info && info.componentStack) &&
                            <>
                                <div style={{ height: theme.spacing(2) }} />
                                <Typography style={{ fontWeight: "bold", marginRight: theme.spacing(0.5) }} color="error" text="Component Stack: " />
                                <Typography style={{
                                    fontFamily: "monospace",
                                    whiteSpace: "pre-wrap",
                                }} text={info.componentStack} color="error" />
                            </>
                    }           
                </div>
            }
            footer={
                <div style={{ textAlign: "right", padding: theme.spacing(2) }}>
                    <Button 
                        color="primary"
                        variant="contained"
                        label="Refresh"
                        onClick={reload}
                    />
                </div>
            }
        />
    );    
};

export default AppError;
