import {render} from "@afw/test";


const Test = (wrapper, Table) => {

    describe("Table tests", () => {

        test("Table renders with minimal props", async () => {

            render(
                <Table id="Table" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Table Test", async () => expect(true).toBe(true));

export default Test;
