// See the 'COPYING' file in the project root for licensing information.
export const Table = ({ columns, rows }) => {

    return (
        <div style={{ display: "table" }}>
            
            {
                columns && columns.map((col, colIndex) =>
                    <div key={colIndex} style={{ display: "table-column" }}>                        
                        <span>{ col.name }</span>
                    </div>
                )
            }
            
            {
                rows && rows.map((row, rowIndex) =>
                    <div key={rowIndex} style={{ display: "table-row" }}>
                        {
                            columns && columns.map((rowCol, index) =>
                                <div key={index} style={{ display: "table-cell" }}>
                                    <span>
                                        { 
                                            rowCol.onRender? 
                                                rowCol.onRender(row, index, rowCol) :                                            
                                                row[rowCol.fieldName]                                                 
                                        }
                                    </span>
                                </div>
                            )
                        }
                    </div>
                )
            }
        </div>
    );
};

export default Table;
