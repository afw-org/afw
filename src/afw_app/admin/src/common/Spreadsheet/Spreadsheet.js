// See the 'COPYING' file in the project root for licensing information.
import {useRef, useEffect, useState, useCallback} from "react";
import jspreadsheet from "jspreadsheet-ce";

import {useOperational} from "@afw/react";

import {useTheme} from "../../hooks";

/**
    INITIALIZATION PARAMETERS

    Parameter	        description
    columns	            Column type, dropdown options, text wrapping, marking, etc.
    colHeaders	        Column header titles
    colWidths	        Column widths: width in px.
    colAlignments	    Column alignments: left, right, center.
    colHeaderClasses    Column custom CSS classes
    defaultColWidth	    Default width for a new column
    minSpareRows	    Minimum number of spare rows
    minSpareCols	    Minimum number of spare cols
    minDimensions	    Minimum table dimensions: [cols,rows]
    contextMenu	        Context menu content: function() { return customMenu }
    columnSorting	    Allow column sorting: bool
    columnResize	    Allow column resizing: bool
    rowDrag	            Allow row dragging: bool
    editable	        Allow table edition: bool
    allowInsertRow	    Allow insert a new row: bool
    allowManualInsertRow	Allow user to insert a new row: bool
    allowInsertColumn	Allow insert a new column: bool
    allowManualInsertColumn	Allow user to create a new column: bool
    allowDeleteRow	    Allow delete a row: bool
    allowDeleteColumn	Allow delete a column: bool
    wordWrap	        Global text wrapping: bool
    csvFileName	        CSV default file name: string
    selectionCopy	    Allow selection copy: bool
    tableOverflow	    Allow table overflow: bool
    tableHeight	        Force the max height of the table
    tableWidth	        Force the max width of the table
    allowComments	    Allow comments over the cells
    toolbar	            Add custom toolbars


    EVENTS

    Event	        description
    onload	        This method is called when the method setData
    onbeforechange	Before a column value is changed.
    onchange	    After a column value is changed.
    onafterchange	After all change events is performed.
    oninsertrow	    After a new row is inserted.
    ondeleterow	    After a row is excluded.
    oninsertcolumn	After a new column is inserted.
    ondeletecolumn	After a column is excluded.
    onselection	    On the selection is changed.
    onsort	        After a colum is sorted.
    onresize	    After a colum is resized.
    onmoverow	    After a row is moved to a new position.
    onfocus	        On table focus
    onblur	        On table blur
 */
export const Spreadsheet = (props) => {

    const theme = useTheme();
    const {editable} = useOperational();
    const containerRef = useRef();
    const el = useRef();
    const [width, setWidth] = useState(0);    

    const {
        id, 
        style, 
        data, 
        columns = [], 
        meta, 
        minDimensions = [1, 1], 
        onCellChanged,
        ...rest
    } = props;    

    useEffect(() => {
        if (theme && theme.palette.mode === "dark")
            import("./jexcel.dark.css");
        else
            import("./jexcel.light.css");
    }, [theme]);

    useEffect(() => {
        if (containerRef.current) {
            setWidth(containerRef.current.offsetWidth);
        }

        const handleResize = () => {
            setWidth(containerRef.current.offsetWidth);
        };

        window.addEventListener("resize", handleResize);

        return () => {
            window.removeEventListener("resize", handleResize);
        };
    }, []);

    const onchange = useCallback((instance, cell, x, y, value, oldValue) => {

        const column = jspreadsheet.getColumnNameFromId([x, y]);
        const meta = el.current.getMeta(column);

        onCellChanged(instance, cell, x, y, value, oldValue, meta);
    }, [onCellChanged]);
    
    useEffect(() => {
        const container = containerRef.current;
        el.current = jspreadsheet(container, { 
            data, columns, meta, minDimensions, defaultColAlign: "left", 
            editable, about: false, onchange,
            ...rest
        });

        /*
            onload	This method is called when the method setData
            onbeforechange	Before a column value is changed.
            onchange	After a column value is changed.
            onafterchange	After all change events is performed.
            oninsertrow	After a new row is inserted.
            ondeleterow	After a row is excluded.
            oninsertcolumn	After a new column is inserted.
            ondeletecolumn	After a column is excluded.
            onselection	On the selection is changed.
            onsort	After a colum is sorted.
            onresize	After a colum is resized.
            onmoverow	After a row is moved to a new position.
            onfocus	On table focus
            onblur	On table blur
         */

        return () => {
            jspreadsheet.destroy(container);
        };
    }, [data, minDimensions, columns, meta, editable, onchange, rest]);

    useEffect(() => {
        if (width && columns) {
            columns.forEach((column, index) => {
                const {width: w} = column;
                if (typeof(w) === "string" && w.indexOf("%") > 0) { 
                    const newWidth = (parseInt(w.substring(0, w.indexOf("%"))) / 100) * width;
                    el.current.setWidth(index, newWidth);
                }
            });
        }
    }, [width, columns]);

    return (
        <div id={id} style={style} ref={containerRef} />
    );
};

export default Spreadsheet;
