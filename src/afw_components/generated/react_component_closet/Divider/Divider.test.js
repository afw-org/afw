import {render} from "@afw/test";


const Test = (wrapper, Divider) => {

    describe("Divider tests", () => {

        test("Divider renders with minimal props", async () => {

            render(
                <Divider id="Divider" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Divider Test", async () => expect(true).toBe(true));

export default Test;
