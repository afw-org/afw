// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import {
    CodeEditor,
    useOperational,
} from "@afw/react";

export const ModelSource = (props) => {

    const {editable} = useOperational(props);
    const {model} = props;

    /* When the model changes, or user changes to "Source" perspective, we stringify the model */    
    const schema = useMemo(() => model.getJSONSchema(), [model]);

    const source = useMemo(() => JSON.stringify(model.toJSON(false), null, 4), [model]);    
    
    const onSourceChanged = (src) => {
        try {
            // try to parse the JSON to ensure it's valid
            JSON.parse(src);

            // use it for the AfwObject
            model.fromJSON(src);
        } catch (error) {
            //eslint-disable-next-line
        }
    };

    return (            
        <CodeEditor                 
            data-testid="admin-admin-models-source"
            style={{ height: "100%" }}
            readOnly={!editable}
            source={source}
            showLineNumbers={true}
            onChanged={onSourceChanged}
            schema={schema}
        />
    );    
};

export default ModelSource;
