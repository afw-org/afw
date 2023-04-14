import {render} from "@afw/test";


const Test = (wrapper, Drawer) => {

    describe("Drawer tests", () => {

        test("Drawer renders with minimal props", async () => {

            render(
                <Drawer id="Drawer" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Drawer Test", async () => expect(true).toBe(true));

export default Test;
