// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen} from "../test-utils";

describe("Authorization Tests", () => {    

    test("Authorization renders", async () => {

        renderRoute("/Admin/Authorization");

        expect(await screen.findByText("Admin")).toBeInTheDocument();
        expect(await screen.findByLabelText("Help")).toBeInTheDocument();
    });
});
