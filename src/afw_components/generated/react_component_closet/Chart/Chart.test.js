import {render} from "@afw/test";


const Test = (wrapper, Chart) => {

    describe("Chart tests", () => {

        test("Chart renders with minimal props", async () => {

            render(
                <Chart id="Chart" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Chart Test", async () => expect(true).toBe(true));

export default Test;
