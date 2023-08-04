import {render} from "@afw/test";


const Test = (wrapper, Tree) => {

    describe("Tree tests", () => {

        test("Tree renders with minimal props", async () => {

            render(
                <Tree id="Tree" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Tree Test", async () => expect(true).toBe(true));

export default Test;
