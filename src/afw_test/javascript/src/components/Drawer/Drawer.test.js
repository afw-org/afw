// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {render} from "@testing-library/react";


const Test = (wrapper, Drawer) => {

    describe("Drawer tests", () => {

        test("Drawer renders with minimal props", async () => {

            render(
                <Drawer id="Drawer" />, 
                { wrapper }
            );

        });
    });
};

export default Test;
