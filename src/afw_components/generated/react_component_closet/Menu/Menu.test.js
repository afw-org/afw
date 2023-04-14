import {render} from "@afw/test";


const Test = (wrapper, Menu) => {

    describe("Menu tests", () => {

        test("Menu renders with minimal props", async () => {

            render(
                <Menu id="Menu" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Menu Test", async () => expect(true).toBe(true));

export default Test;
