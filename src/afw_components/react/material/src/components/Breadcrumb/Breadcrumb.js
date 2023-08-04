// See the 'COPYING' file in the project root for licensing information.
import {Link as RouterLink} from "react-router-dom";

import Breadcrumbs from "@mui/material/Breadcrumbs";
import MuiLink from "@mui/material/Link";
import {useTheme} from "@mui/material/styles";
import Typography from "@mui/material/Typography";

import useMediaQuery from "@mui/material/useMediaQuery";

export const Breadcrumb = ({ items, separator, ...rest }) => {    
    const theme = useTheme();
    
    const small = useMediaQuery(theme.breakpoints.down("md"));

    return (
        <Breadcrumbs {...rest} separator={ separator ? separator : "/"} maxItems={small ? 2 : undefined}>
            {
                items.map((item, index) => {
                    if (index === items.length-1)
                        return (
                            <Typography key={index}>{item.text}</Typography>
                        );
                    else
                        return (
                            <MuiLink 
                                key={index}
                                component={RouterLink}
                                to={item.link}
                                underline="hover"
                            >{item.text}</MuiLink>                        
                        );
                })
            }
        </Breadcrumbs>
    );    
};

export default Breadcrumb;
