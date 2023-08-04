import {render} from "@afw/test";


const Test = (wrapper, Skeleton) => {

    describe("Skeleton tests", () => {

        test("Skeleton renders with minimal props", async () => {

            render(
                <Skeleton id="Skeleton" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Skeleton Test", async () => expect(true).toBe(true));

export default Test;
