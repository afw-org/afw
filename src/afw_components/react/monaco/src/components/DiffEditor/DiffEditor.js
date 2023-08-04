// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DiffEditor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {useState, useEffect, useRef} from "react";

import {useMonaco} from "../../hooks";

import {Button} from "@afw/react";

/**
 * Implementation Id : DiffEditor
 * Category          : editors
 * 
 * A component that renders a rich text editor.
 * 
 * This component displays a rich text editor that allows the user to
 * manually edit the source for a particular piece of data.
 * 
 */
export const DiffEditor = (props) => {

    const containerRef = useRef();
    const [editor, setEditor] = useState();
    const [navigator, setNavigator] = useState();

    const {monaco, theme} = useMonaco();

    const {original, modified, showLineNumbers, showMinimap, style} = props;

    useEffect(() => {
        if (monaco) {
            const editor = monaco.editor.createDiffEditor(containerRef.current, {
                theme,
                lineNumbers: (showLineNumbers ? "on" : "off"),
                scrollBeyondLastLine: false,
                wordWrap: "on",
                minimap: {
                    enabled: showMinimap
                }
            });

            const originalModel = monaco.editor.createModel(original);
            const modifiedModel = monaco.editor.createModel(modified);

            editor.setModel({ original: originalModel, modified: modifiedModel });

            setEditor(editor);

            return () => editor.dispose();
        }
    }, [monaco]);

    useEffect(() => {
        if (editor) {
            editor.updateOptions({ 
                lineNumbers: (showLineNumbers ? "on" : "off"), 
                wordWrap: "on", 
                scrollBeyondLastLine: false,
                minimap: {
                    enabled: showMinimap
                }
            });
        } 
    }, [editor, showLineNumbers, showMinimap]);

    useEffect(() => {
        if (monaco && editor) {
            const navigator = monaco.editor.createDiffNavigator(editor, {
                followsCaret: true,
                ignoreCharChanges: true
            });

            setNavigator(navigator);
        }
    }, [monaco, editor]);

    return (
        <div style={style}>
            <div style={{ display: "flex", justifyContent: "center" }}>
                <Button 
                    type="icon"
                    label="Previous Change"
                    icon="navigate_before" 
                    tooltip="Previous Change"
                    disabled={!navigator}
                    onClick={() => navigator.previous()} 
                />
                <Button 
                    type="icon"
                    label="Next Change"
                    icon="navigate_next" 
                    tooltip="Next Change"
                    disabled={!navigator}
                    onClick={() => navigator.next()} 
                />
            </div>
            <div ref={containerRef} style={{ width: "100%", height: "100%" }} />
        </div>
    );
};

export default DiffEditor;
