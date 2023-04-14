import {render} from "@afw/test";


const Test = (wrapper, Toolbar) => {

    describe("Toolbar tests", () => {

        test("Toolbar renders with minimal props", async () => {

            render(
                <Toolbar id="Toolbar" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Toolbar Test", async () => expect(true).toBe(true));

export default Test;
