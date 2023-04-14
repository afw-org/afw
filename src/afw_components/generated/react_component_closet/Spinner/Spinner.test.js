import {render} from "@afw/test";


const Test = (wrapper, Spinner) => {

    describe("Spinner tests", () => {

        test("Spinner renders with minimal props", async () => {

            render(
                <Spinner id="Spinner" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Spinner Test", async () => expect(true).toBe(true));

export default Test;
