// See the 'COPYING' file in the project root for licensing information.
import {Spinner} from "@afw/react";

import {useApplication} from "../hooks";

function Loading(props) {

    const {onHandleAppRequestError} = useApplication();

    if (props.error) {
        onHandleAppRequestError(props.error);

        return (
            <div>Failed to load content from server.</div>
        );
    } else {
        return (
            <Spinner size="small" text="Loading Content..." />
        );
    }
}

export default Loading;
