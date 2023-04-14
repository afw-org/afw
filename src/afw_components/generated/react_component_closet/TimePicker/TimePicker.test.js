import {render} from "@afw/test";


const Test = (wrapper, TimePicker) => {

    describe("TimePicker tests", () => {

        test("TimePicker renders with minimal props", async () => {

            render(
                <TimePicker id="TimePicker" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads TimePicker Test", async () => expect(true).toBe(true));

export default Test;
