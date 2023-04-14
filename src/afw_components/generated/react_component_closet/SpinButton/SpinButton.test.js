import {render} from "@afw/test";


const Test = (wrapper, SpinButton) => {

    describe("SpinButton tests", () => {

        test("SpinButton renders with minimal props", async () => {

            render(
                <SpinButton id="SpinButton" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads SpinButton Test", async () => expect(true).toBe(true));

export default Test;
