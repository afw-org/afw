import {render} from "@afw/test";


const Test = (wrapper, Responsive) => {

    describe("Responsive tests", () => {

        test("Responsive renders with minimal props", async () => {

            render(
                <Responsive id="Responsive" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Responsive Test", async () => expect(true).toBe(true));

export default Test;
