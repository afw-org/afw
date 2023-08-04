import {render} from "@afw/test";


const Test = (wrapper, Dropdown) => {

    describe("Dropdown tests", () => {

        test("Dropdown renders with minimal props", async () => {

            render(
                <Dropdown id="Dropdown" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Dropdown Test", async () => expect(true).toBe(true));

export default Test;
