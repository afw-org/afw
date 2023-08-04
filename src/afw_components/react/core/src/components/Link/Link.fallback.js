// See the 'COPYING' file in the project root for licensing information.
import {Link as RouterLink} from "react-router-dom";

export const Link = ({ url, uriComponents, text, onClick }) => {

    let _url;               
    if (uriComponents)
        _url = "/" + uriComponents.map(uriComponent => encodeURIComponent(uriComponent)).join("/");            
    else 
        _url = url;

    if (!_url || !text)
        return null;
        
    return (
        url.startsWith("/") ?
            <RouterLink
                to={_url}                
                onClick={onClick}
            >{text}</RouterLink>  :            
            <a href={_url}>{text}</a>
    );

};

export default Link;
