import {render} from "@afw/test";


const Test = (wrapper, Autocomplete) => {

    describe("Autocomplete tests", () => {

        test("Autocomplete renders with minimal props", async () => {

            render(
                <Autocomplete id="Autocomplete" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Autocomplete Test", async () => expect(true).toBe(true));

export default Test;
