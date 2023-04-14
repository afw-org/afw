import {render} from "@afw/test";


const Test = (wrapper, TimeDuration) => {

    describe("TimeDuration tests", () => {

        test("TimeDuration renders with minimal props", async () => {

            render(
                <TimeDuration id="TimeDuration" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads TimeDuration Test", async () => expect(true).toBe(true));

export default Test;
