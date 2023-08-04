import {render} from "@afw/test";


const Test = (wrapper, Snackbar) => {

    describe("Snackbar tests", () => {

        test("Snackbar renders with minimal props", async () => {

            render(
                <Snackbar id="Snackbar" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Snackbar Test", async () => expect(true).toBe(true));

export default Test;
