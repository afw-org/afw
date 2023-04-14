import {render} from "@afw/test";


const Test = (wrapper, Tooltip) => {

    describe("Tooltip tests", () => {

        test("Tooltip renders with minimal props", async () => {

            render(
                <Tooltip id="Tooltip" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Tooltip Test", async () => expect(true).toBe(true));

export default Test;
