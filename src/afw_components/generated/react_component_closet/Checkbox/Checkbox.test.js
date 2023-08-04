import {render} from "@afw/test";


const Test = (wrapper, Checkbox) => {

    describe("Checkbox tests", () => {

        test("Checkbox renders with minimal props", async () => {

            render(
                <Checkbox id="Checkbox" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Checkbox Test", async () => expect(true).toBe(true));

export default Test;
