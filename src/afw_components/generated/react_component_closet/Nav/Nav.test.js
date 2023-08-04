import {render} from "@afw/test";


const Test = (wrapper, Nav) => {

    describe("Nav tests", () => {

        test("Nav renders with minimal props", async () => {

            render(
                <Nav id="Nav" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Nav Test", async () => expect(true).toBe(true));

export default Test;
