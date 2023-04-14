// See the 'COPYING' file in the project root for licensing information.
export const Responsive = ({ rows }) => {
    return (
        <div>
            {
                rows && rows.map((row, rowIndex) => 
                    <div key={rowIndex}>
                        { 
                            row.columns && row.columns.map((column) =>  
                                column.contains                            
                            )
                        }
                    </div>
                )
            }
        </div>
    );
};

export const ResponsiveRow = ({ columns }) => {
    return (
        columns && columns.map(column =>
            column.contains
        )
    );
};

export const ResponsiveCol = ({ column }) => {
    return column ? column.contains : null;
};

export default Responsive;
