// See the 'COPYING' file in the project root for licensing information.
import {useState, useRef} from "react";

import {ctx} from "@afw/react";
import {    
    useOperational,
    TextField,
    Typography
} from "@afw/react";

export const EditableLabel = (props) => {

    const [editing, setEditing] = useState();
    const textFieldRef = useRef(); 

    const {editable} = useOperational(props);

    const {value, multiline, onChanged, onValidate, onRenderLabel, placeholder} = props;

    const onEdit = () => {
        setEditing(true);
        if (textFieldRef.current)
            textFieldRef.current.focus();
    };

    const onEnter = () => {
        setEditing(false);

        if (props.onEnter)
            props.onEnter();
    };

    let component;

    /* 
        If we are "editing", or it's the first time loading without a value, 
            then present a TextField to enter data.
     */
    if (editing) {                        
        component =
            <TextField 
                inputRef={textFieldRef}
                value={value ? value : ""}
                multiline={multiline}
                onBlur={() => setEditing(false)}
                onChanged={onChanged}
                onValidate={onValidate}
                //eslint-disable-next-line
                autoFocus={true}
                onEnter={multiline ? undefined : onEnter}
                disabled={editable === false}
            />;                                
    } else {
        if (onRenderLabel) 
            component = onRenderLabel(value);
        else {
            let val = value;
            if (!val || val.length === 0)
                val = placeholder;

            component = (
                <Typography 
                    text={val ? val : ""} 
                    size="5"
                    color="textSecondary"
                />
            );
        }

        /* now wrap our label component inside a clickable area */
        component = 
            <div 
                data-component-type={ctx(props, "EditableLabel")}
                style={{ minHeight: "32px" }}
                onClick={editable === false ? () => {} : onEdit}
                onKeyPress={editable === false ? () => {} : onEdit}
                role="button"
                tabIndex="0"
            >
                {component}
            </div>;
    }

    return component;
};

export default EditableLabel;
