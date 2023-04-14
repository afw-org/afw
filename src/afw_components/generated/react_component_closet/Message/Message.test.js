import {render} from "@afw/test";


const Test = (wrapper, Message) => {

    describe("Message tests", () => {

        test("Message renders with minimal props", async () => {

            render(
                <Message id="Message" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Message Test", async () => expect(true).toBe(true));

export default Test;
