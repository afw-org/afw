// See the 'COPYING' file in the project root for licensing information.
import MuiTypography from "@mui/material/Typography";


/* internal mapping of size to framework-specific size */
const sizeMap = {
    "1" : "caption",    
    "2" : "caption",    
    "3" : "body2",
    "4" : "body1",    
    "5" : "subtitle1",    
    "6" : "h6",    
    "7" : "h5",    
    "8" : "h4",    
    "9" : "h3",    
    "10": "h2",    
    "11": "h1",    
};

export const Typography = ({ size = "4", text, paragraph, sx, ...rest }) => {
    if (!text)
        return null;

    const theSize = sizeMap[size];

    /*
     * MUI removed the `paragraph` prop entirely (its own suggested
     * replacement, straight from their migration guide: component="p"
     * plus the bottom margin it used to apply for you).
     */
    return (
        <MuiTypography
            {...rest}
            variant={theSize}
            component={paragraph ? "p" : rest.component}
            sx={paragraph ? { marginBottom: 2, ...sx } : sx}
        >
            {text}
        </MuiTypography>
    );
};

export default Typography;
