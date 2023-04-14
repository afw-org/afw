import {render} from "@afw/test";


const Test = (wrapper, Paper) => {

    describe("Paper tests", () => {

        test("Paper renders with minimal props", async () => {

            render(
                <Paper id="Paper" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Paper Test", async () => expect(true).toBe(true));

export default Test;
