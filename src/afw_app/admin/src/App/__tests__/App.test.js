// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, screen} from "../../test-utils";
import {waitForSpinner, mswPostCallback, mswGetCallback, server, http, HttpResponse} from "../../test-utils";
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
            http.post("/afw", ({request}) => {
                mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.text("Bad Gateway", {status: 502, statusText: "Bad Gateway"});
            }),
            http.get("/*", ({request}) => {
                mswGetCallback("/afw", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.text("Bad Gateway", {status: 502, statusText: "Bad Gateway"});
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
