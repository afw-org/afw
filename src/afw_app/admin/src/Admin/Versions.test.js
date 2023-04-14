// See the 'COPYING' file in the project root for licensing information.
import {render, screen} from "./test-utils";
import Versions from "./Versions";

import versions from "@afw/test/build/cjs/__mocks__/retrieve_objects/afw/_AdaptiveVersionInfo_.json";

describe("Versions Tests", () => {    

    test("Show Versions", async () => {

        render( <Versions /> );        

        expect(await screen.findByText("Version Information")).toBeInTheDocument();

        for (const version of versions.result) {
            expect(await screen.findByTestId(version.id)).toBeInTheDocument();
        }
    });

});
