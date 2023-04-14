import {render} from "@afw/test";


const Test = (wrapper, Toggle) => {

    describe("Toggle tests", () => {

        test("Toggle renders with minimal props", async () => {

            render(
                <Toggle id="Toggle" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Toggle Test", async () => expect(true).toBe(true));

export default Test;
