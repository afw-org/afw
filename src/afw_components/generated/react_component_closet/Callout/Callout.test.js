import {render} from "@afw/test";


const Test = (wrapper, Callout) => {

    describe("Callout tests", () => {

        test("Callout renders with minimal props", async () => {

            render(
                <Callout id="Callout" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Callout Test", async () => expect(true).toBe(true));

export default Test;
