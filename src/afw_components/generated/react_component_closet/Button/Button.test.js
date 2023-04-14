import {render} from "@afw/test";


const Test = (wrapper, Button) => {

    describe("Button tests", () => {

        test("Button renders with minimal props", async () => {

            render(
                <Button id="Button" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Button Test", async () => expect(true).toBe(true));

export default Test;
