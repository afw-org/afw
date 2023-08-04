// See the 'COPYING' file in the project root for licensing information.
import {render} from "../test-utils";
import {MemoryRouter} from "react-router-dom";
import Authorization from "./Authorization";

describe("Authorization Tests", () => {    

    test("Authorization renders", async () => {
        
        render( 
            <MemoryRouter initialEntries={[ "/Admin/Authorization" ]}>
                <Authorization /> 
            </MemoryRouter>
        );    
               
    });
});
