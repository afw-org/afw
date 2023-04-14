import {render} from "@afw/test";


const Test = (wrapper, Box) => {

    describe("Box tests", () => {

        test("Box renders with minimal props", async () => {

            render(
                <Box id="Box" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Box Test", async () => expect(true).toBe(true));

export default Test;
