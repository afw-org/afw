// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, screen} from "../../test-utils";
import {waitForSpinner, mswPostCallback, mswGetCallback, server, rest} from "../../test-utils";
import App from "../App";


describe("App Tests", () => {   
    
    beforeAll(() => {
        mswPostCallback.mockClear();
    });

    test("App renders", async () => {
     
        render( <App /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                   
        await screen.findByRole("main", { hidden: true });

        await waitForSpinner();          
    });

    test("Handle error when unable to fetch data", async () => {        

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                mswPostCallback("/afw", req, res, ctx);
                return res(
                    ctx.status(502, "Bad Gateway"),    
                    ctx.text("Bad Gateway")                
                );
            }),
            rest.get("/*", (req, res, ctx) => {    
                mswGetCallback("/afw", req, res, ctx); 
                return res(
                    ctx.status(502, "Bad Gateway"),                    
                    ctx.text("Bad Gateway")
                );
            })
        );
        
        render(            
            <App />             
        );
        
        await waitForSpinner();
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled()); 
        await waitForSpinner();

        await screen.findByText("Error Loading Application Data");                
    });   
});
