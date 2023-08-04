import {render} from "@afw/test";


const Test = (wrapper, ToggleButtons) => {

    describe("ToggleButtons tests", () => {

        test("ToggleButtons renders with minimal props", async () => {

            render(
                <ToggleButtons id="ToggleButtons" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads ToggleButtons Test", async () => expect(true).toBe(true));

export default Test;
