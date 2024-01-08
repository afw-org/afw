// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Script
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    CodeEditor, 
    Typography,
    useOperational,
    useTheme,
} from "@afw/react";

import {ExpandableComponent} from "../../../utils";


/*
 * Script implements a dataType=script by displaying the value inside 
 * a CodeEditor, along with a label and description.  It uses a heuristic
 * of displaying just a 300px height view, then provides a button to 
 * expand the view to whole screen for script editing.
 */
export const Script = (props) => {
    const theme = useTheme();
    const {editable} = useOperational(props);
               
    const {id, value, valueMeta = {}, onChanged} = props;
    const {label, brief} = valueMeta;

    return (
        <ExpandableComponent>
            <div style={{ display: "flex", flexDirection: "column", height: "100%", minHeight: "300px" }}>
                {
                    editable && label &&
                        <Typography style={{ marginBottom: theme.spacing(0.5) }} size="2" color="textSecondary" text={label} />
                }
                <CodeEditor 
                    style={{ flex: 1, height: "100%" }}
                    id={id}
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={true}
                    readOnly={!editable}
                    source={value}
                    language="afw"
                    wordWrap={true}
                    showMinimap={true}
                    onChanged={onChanged}
                />
                {
                    editable && brief &&
                        <Typography style={{ marginTop: theme.spacing(0.5) }} size="1" text={brief} />
                }
            </div>
        </ExpandableComponent>
    );  
};

Script.displayName = "Script";

export default Script;
