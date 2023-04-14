import {render} from "@afw/test";


const Test = (wrapper, Native) => {

    describe("Native tests", () => {

        test("Native renders with minimal props", async () => {

            render(
                <Native id="Native" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Native Test", async () => expect(true).toBe(true));

export default Test;
