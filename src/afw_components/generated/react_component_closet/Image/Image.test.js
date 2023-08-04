import {render} from "@afw/test";


const Test = (wrapper, Image) => {

    describe("Image tests", () => {

        test("Image renders with minimal props", async () => {

            render(
                <Image id="Image" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Image Test", async () => expect(true).toBe(true));

export default Test;
