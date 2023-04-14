import {render} from "@afw/test";


const Test = (wrapper, Slider) => {

    describe("Slider tests", () => {

        test("Slider renders with minimal props", async () => {

            render(
                <Slider id="Slider" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Slider Test", async () => expect(true).toBe(true));

export default Test;
