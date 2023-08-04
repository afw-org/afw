// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import MuiIcon from "@mui/material/Icon";

import {importIcon, canImportIcon} from "@afw/react";

export const Icon = (props) => {

    const [icon, setIcon] = useState();
    const [loading, setLoading] = useState();

    const {
        id, "data-testid": dataTestId, "data-component-type": dataComponentType, 
        iconName, className, color, style, fontSize, variant, disabled
    } = props;

    useEffect(() => {
        let isMounted = true;

        const fetchIcon = async () => {
            if (isMounted)
                setLoading(true);

            const icon = await importIcon(iconName);
            if (icon && isMounted) {
                setIcon(icon);
            }          

            if (isMounted)
                setLoading(false);                    
        };

        if (iconName && canImportIcon(iconName))
            fetchIcon();
    
        return () => { isMounted = false; };
    }, [iconName]);    
    
    if (loading)
        return null;

    if (icon) {                    
        const AsyncIcon = icon;
        return (
            <AsyncIcon 
                color={color} 
                style={style}  
            />
        );
    }

    return (
        <MuiIcon 
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            className={className}
            style={style}
            color={color}
            fontSize={fontSize}
            variant={variant}
            disabled={disabled}
        >
            {iconName}
        </MuiIcon>
    );     
};

export default Icon;
