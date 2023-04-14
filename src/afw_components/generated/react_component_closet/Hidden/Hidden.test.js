import {render} from "@afw/test";


const Test = (wrapper, Hidden) => {

    describe("Hidden tests", () => {

        test("Hidden renders with minimal props", async () => {

            render(
                <Hidden id="Hidden" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Hidden Test", async () => expect(true).toBe(true));

export default Test;
