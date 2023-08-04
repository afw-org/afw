import {render} from "@afw/test";


const Test = (wrapper, CodeEditor) => {

    describe("CodeEditor tests", () => {

        test("CodeEditor renders with minimal props", async () => {

            render(
                <CodeEditor id="CodeEditor" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads CodeEditor Test", async () => expect(true).toBe(true));

export default Test;
