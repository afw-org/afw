// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Template
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {
    CodeEditor,
    TextField,
    Typography,
    useOperational,
    useTheme,
} from "@afw/react";

import {ExpandableComponent} from "../../../utils";

/*
 * Handles dataType=template
 */
export const Template = (props) => {

    const theme = useTheme();
    const {editable} = useOperational(props);
               
    const {id, value, valueMeta = {}, autoFocus, onChanged} = props;
    const {label, brief} = valueMeta;

    return (
        <ExpandableComponent
            expandedChildren={
                <div style={{ display: "flex", flexDirection: "column", height: "100%", minHeight: "100px" }}>
                    {
                        editable && label &&
                            <Typography style={{ marginBottom: theme.spacing(0.5) }} size="2" color="textSecondary" text={label} />
                    }
                    <CodeEditor 
                        style={{ flex: 1, height: "100%" }}
                        id={id}
                        label={label}
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        readOnly={!editable}
                        source={value}
                        language="afw"
                        wordWrap={true}
                        showMinimap={false}
                        onChanged={onChanged}
                        showGutter={false}
                    />
                    {
                        editable && brief &&
                            <Typography style={{ marginTop: theme.spacing(0.5) }} size="1" text={brief} />
                    }
                </div>
            }
        >
            <TextField 
                id={id}                
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta}
                description={brief}
                type="text"
                value={value}
                multiline={true}           
                onChanged={onChanged}  
            /> 
        </ExpandableComponent>
    );
};

Template.displayName = "Template";

export default Template;
