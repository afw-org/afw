import {render} from "@afw/test";


const Test = (wrapper, Link) => {

    describe("Link tests", () => {

        test("Link renders with minimal props", async () => {

            render(
                <Link id="Link" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Link Test", async () => expect(true).toBe(true));

export default Test;
