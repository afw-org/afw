// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Table
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */
import {useReducer, useEffect} from "react";

import MuiTable from "@mui/material/Table";
import MuiTableSortLabel from "@mui/material/TableSortLabel";
import MuiTableBody from "@mui/material/TableBody";
import MuiTableCell from "@mui/material/TableCell";
import MuiTableRow from "@mui/material/TableRow";
import MuiTableHead from "@mui/material/TableHead";
import Checkbox from "@mui/material/Checkbox";
import MuiTablePagination from "@mui/material/TablePagination";
import TextField from "@mui/material/TextField";
import IconButton from "@mui/material/IconButton";

import ViewColumn from "@mui/icons-material/ViewColumn";
import FilterList from "@mui/icons-material/FilterList";

import CheckCircleOutline from "@mui/icons-material/CheckCircleOutline";
import CheckCircle from "@mui/icons-material/CheckCircle";

import {useClasses} from "@afw/react";

/* Checkbox style applied to table rows when hovering over the selection */
const styles = {
    CheckboxHover : {
        opacity: "0.1",
        "&:hover": {
            opacity: "0.5"
        }
    },
    CompactStyle: {
        borderBottom: "0px", 
        paddingTop: "7px", 
        paddingBottom: "7px"
    },
};

const initialState = {
    selected:       [],
    page:           0,
    rowsPerPage:    10,
    order:          true,
    filter:         "",
};

const reducer = (state, action) => {

    switch (action.type) {

    case "FILTER_CHANGE":
        return {
            ...state,
            ...action,
        };

    case "ROWS_CHANGE":
        return {
            ...state,
            selected: [],
        };

    case "SORT_COLUMN":
        return {
            ...state,
            ...action,
        };

    case "SELECT_ALL_ROWS":
    case "SELECT_ROW":
        return {
            ...state,
            ...action,
        };

    case "PAGE_CHANGE":
        return {
            ...state,
            ...action,
        };

    case "ROWS_PER_PAGE_CHANGE":
        return {
            ...state,
            ...action,
        };

    default:
        return state;
    }
};

const TableHeader = (props) => {

    const {
        columns,
        rows,
        selected,
        selectionMode,
        onSelectAllRows,
        onSortColumn,
    } = props;

    return (
        <MuiTableHead>
            <MuiTableRow>
                {
                    (selectionMode !== "none") &&
                        <MuiTableCell padding="checkbox">
                            { (selectionMode !== "single") &&
                                <Checkbox 
                                    inputProps={{
                                        "aria-label": "Select All Rows"
                                    }}
                                    icon={<CheckCircleOutline style={{ opacity: "0.5" }} />}   
                                    checkedIcon={<CheckCircle />}
                                    checked={rows && (selected.length === rows.length)}
                                    onChange={(event, checked) => onSelectAllRows(checked)}
                                    disabled={rows?.length === 0}
                                />
                            }
                        </MuiTableCell>
                }
                {
                    columns?.map((column, index) => 
                        <MuiTableCell 
                            key={index}                                            
                        >
                            <MuiTableSortLabel
                                onClick={column.getValue ? () => onSortColumn(column, index) : undefined}
                            >
                                {column.name}
                            </MuiTableSortLabel>
                        </MuiTableCell>
                    )
                }
            </MuiTableRow>
        </MuiTableHead>
    );
};

const TableBody = (props) => {

    const {
        columns,
        compact,
        hover,
        onRowInvoked,
        onSelectRow,        
        rows,
        selected = [],
        selectionMode,
    } = props;

    const classes = useClasses(styles);

    return (
        <MuiTableBody>
            {
                rows?.map((row, rowIndex) =>
                    <MuiTableRow                         
                        key={rowIndex}
                        hover={hover}
                        selected={selected.includes(row)}
                        onClick={() => onSelectRow(row, rowIndex)}
                        onDoubleClick={() => onRowInvoked(row, rowIndex)}
                    >
                        {
                            (selectionMode !== "none") &&
                                <MuiTableCell                                             
                                    className={compact ? classes.CompactStyle : undefined}
                                    padding="checkbox" 
                                    width={16}
                                >
                                    <Checkbox                                                     
                                        inputProps={{
                                            "aria-label": "Select Table Row " + rowIndex
                                        }}
                                        icon={<CheckCircleOutline className={classes.CheckboxHover} />}
                                        checkedIcon={<CheckCircle />}
                                        checked={selected.includes(row)}
                                    />
                                </MuiTableCell>
                        }
                        {
                            columns?.map((column, colIndex) => 
                                <MuiTableCell 
                                    className={compact ? classes.CompactStyle : undefined}
                                    style={{ ...column.style }}                                                
                                    key={colIndex}
                                >
                                    {
                                        column.onRender ? 
                                            column.onRender(row, rowIndex, column) : 
                                            row[column.fieldName]
                                    }
                                </MuiTableCell>
                            )
                        }
                    </MuiTableRow>
                )
            }
        </MuiTableBody>
    );
};

const TableFooter = (props) => {

    const {
        filter,
        onChangePage,
        onChangeRowsPerPage,
        onChangeFilter,
        page,
        pagination,
        picker,
        rowsPerPage,
        rows,
    } = props;

    return (
        <div style={{ display: "flex" }}>
            <div style={{ flex: 1, display: "flex" }}>
                <div>
                    {
                        picker &&
                        <div style={{ display: "inline-block", paddingRight: "40px", verticalAlign: "bottom" }}>
                            <IconButton> 
                                <ViewColumn />
                            </IconButton>
                        </div>
                    }
                </div>
                <div>
                    {
                        filter &&
                        <div>
                            <TextField
                                variant="standard"
                                disabled={!rows || rows?.length === 0}
                                InputProps={{
                                    startAdornment: (
                                        <FilterList />
                                    ),                                        
                                }}
                                value={filter}
                                onChanged={onChangeFilter}
                            />
                        </div>
                    }
                </div>
            </div>
            <div>
                {
                    pagination &&
                        <MuiTablePagination
                            component="div"
                            count={rows?.length || 0}
                            rowsPerPage={rowsPerPage}
                            page={page}
                            onChangePage={(event, page) => onChangePage(page)}
                            onChangeRowsPerPage={(event) => onChangeRowsPerPage(event.target.value)}
                            rowsPerPageOptions={[ 5, 10, 20, 25, 50, 100 ]}
                            labelRowsPerPage="Objects per page:"
                        />
                }
            </div>            
        </div>
    );
};

export const Table = (props) => {

    const [state, dispatch] = useReducer(reducer, initialState);

    const {
        id,
        "aria-label": ariaLabel,
        "data-testid": dataTestId,
        "data-component-type": dataComponentType,
        className,
        columns,
        compact,
        filter,
        hover = true,        
        isHeaderVisible = true,
        onSelectionChanged,
        onRowInvoked = () => undefined,
        pagination,
        picker,
        selectionMode,
        rows,
        style,
    } = props;

    useEffect(() => {
        dispatch({ type: "ROWS_CHANGE", rows });
    // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [rows]);

    const onSortColumn = (column) => {
        let {orderBy, order} = state;

        if (orderBy === column)
            order = !order;
        else
            orderBy = column;

        dispatch({ type: "SORT_COLUMN", orderBy, order });
    };

    const onSelectAllRows = (checked) => {
        let selected;

        if (checked)
            selected = [ ...rows ];
        else 
            selected = [];

        dispatch({ type: "SELECT_ALL_ROWS", selected });

        if (onSelectionChanged)
            onSelectionChanged(selected);    
    };

    const onSelectRow = (row) => {

        if (selectionMode === "none")
            return;
            
        let selected = state.selected;

        if (selected.includes(row))
            selected = selected.filter(r => r !== row);
        else {
            if (selectionMode === "single")
                selected = [ row ];
            else
                selected = [ ...selected, row ];        
        }

        dispatch({ type: "SELECT_ROW", selected });

        if (onSelectionChanged)
            onSelectionChanged(selected);
    };

    return (
        <div 
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            className={className}
            style={style}
        >
            <MuiTable size={compact ? "small" : "medium"} aria-label={ariaLabel}>
                { 
                    isHeaderVisible && 
                        <TableHeader 
                            rows={rows}
                            columns={columns}
                            selected={state.selected}
                            selectionMode={selectionMode}                            
                            onSortColumn={onSortColumn}
                            onSelectAllRows={onSelectAllRows}
                        /> 
                }
                <TableBody 
                    columns={columns}
                    compact={compact}
                    hover={hover}
                    onSelectRow={onSelectRow}
                    onRowInvoked={onRowInvoked}
                    rows={rows}                    
                    selected={state.selected}
                    selectionMode={selectionMode}                    
                />                
            </MuiTable>
            <TableFooter 
                filter={filter}
                onChangePage={page => dispatch({ type: "PAGE_CHANGE", page })}
                onChangeRowsPerPage={rowsPerPage => dispatch({ type: "ROWS_PER_PAGE_CHANGE", rowsPerPage })}
                onChangeFilter={filter => dispatch({ type: "FILTER_CHANGE", filter })}
                page={state.page}
                pagination={pagination}
                picker={picker}
                rowsPerPage={state.rowsPerPage}
                rows={rows}
            />
        </div>
    );

};

export default Table;
