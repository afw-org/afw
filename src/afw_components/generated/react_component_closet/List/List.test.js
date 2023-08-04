import {render} from "@afw/test";


const Test = (wrapper, List) => {

    describe("List tests", () => {

        test("List renders with minimal props", async () => {

            render(
                <List id="List" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads List Test", async () => expect(true).toBe(true));

export default Test;
