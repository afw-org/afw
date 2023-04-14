import {render} from "@afw/test";


const Test = (wrapper, Collapsible) => {

    describe("Collapsible tests", () => {

        test("Collapsible renders with minimal props", async () => {

            render(
                <Collapsible id="Collapsible" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Collapsible Test", async () => expect(true).toBe(true));

export default Test;
