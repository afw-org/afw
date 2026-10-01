// See the 'COPYING' file in the project root for licensing information.
import {useNavigation} from "../../hooks";

export const Link = ({ url, uriComponents, text, onClick }) => {

    const {Link: NavigationLink} = useNavigation();

    let _url;
    if (uriComponents)
        _url = "/" + uriComponents.map(uriComponent => encodeURIComponent(uriComponent)).join("/");
    else
        _url = url;

    if (!_url || !text)
        return null;

    return (
        _url.startsWith("/") ?
            <NavigationLink
                to={_url}
                onClick={onClick}
            >{text}</NavigationLink>  :
            <a href={_url}>{text}</a>
    );

};

export default Link;
