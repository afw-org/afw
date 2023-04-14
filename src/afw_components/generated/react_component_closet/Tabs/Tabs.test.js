import {render} from "@afw/test";


const Test = (wrapper, Tabs) => {

    describe("Tabs tests", () => {

        test("Tabs renders with minimal props", async () => {

            render(
                <Tabs id="Tabs" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Tabs Test", async () => expect(true).toBe(true));

export default Test;
