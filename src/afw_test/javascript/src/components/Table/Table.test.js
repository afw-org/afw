// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Table) => {

    describe("Table tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Table    
                    columns={[
                        {
                            key: "col1",
                            name: "Column 1",
                            fieldName: "col1"   
                        },
                        {
                            key: "col2",
                            name: "Column 2",
                            fieldName: "col2"   
                        },
                        {
                            key: "col3",
                            name: "Column 3",
                            fieldName: "col3"   
                        },
                    ]}
                    rows={[
                        {
                            col1: "row 1, col 1",
                            col2: "row 1, col 2",
                            col3: "row 1, col 3"
                        },
                        {
                            col1: "row 2, col 1",
                            col2: "row 2, col 2",
                            col3: "row 2, col 3"
                        },
                        {
                            col1: "row 3, col 1",
                            col2: "row 3, col 2",
                            col3: "row 3, col 3"
                        },
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/row 2, col 2/)).toBeTruthy();               
        });
        
    });
};

export default Test;
