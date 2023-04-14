import {render} from "@afw/test";


const Test = (wrapper, TimezonePicker) => {

    describe("TimezonePicker tests", () => {

        test("TimezonePicker renders with minimal props", async () => {

            render(
                <TimezonePicker id="TimezonePicker" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads TimezonePicker Test", async () => expect(true).toBe(true));

export default Test;
