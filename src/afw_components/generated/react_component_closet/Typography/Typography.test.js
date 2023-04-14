import {render} from "@afw/test";


const Test = (wrapper, Typography) => {

    describe("Typography tests", () => {

        test("Typography renders with minimal props", async () => {

            render(
                <Typography id="Typography" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Typography Test", async () => expect(true).toBe(true));

export default Test;
