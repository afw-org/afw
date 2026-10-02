// See the 'COPYING' file in the project root for licensing information.
import {useLocation} from "@tanstack/react-router";

/**
 * useLocationHash()
 *
 * The location's hash in React Router 5's form: "#tree", or "" when there
 * is none. TanStack's location.hash has no leading "#"; code that builds
 * links by appending the hash (the model editor's breadcrumbs and tables)
 * keeps using this form.
 */
export const useLocationHash = () => {
    const {hash} = useLocation();
    return hash ? "#" + hash : "";
};
