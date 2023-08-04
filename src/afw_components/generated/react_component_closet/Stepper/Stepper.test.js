import {render} from "@afw/test";


const Test = (wrapper, Stepper) => {

    describe("Stepper tests", () => {

        test("Stepper renders with minimal props", async () => {

            render(
                <Stepper id="Stepper" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Stepper Test", async () => expect(true).toBe(true));

export default Test;
