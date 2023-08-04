// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";
import {waitForSpinner, mswPostCallback, mswGetCallback, screen, server, rest} from "../../test-utils";

import {render, waitFor} from "../../test-utils";

import Home from "../Home";

describe("Home Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Home renders", async () => {

        render( <Home /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());  
        await waitForSpinner();                      

        expect(await screen.findByTestId("admin-home")).toBeInTheDocument();              
    });

    test("Home page shows appropriate content", async () => {
        const history = createMemoryHistory();
        history.push("/Home");

        render(
            <Router history={history}>
                <Home /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();        

        expect(await screen.findByTestId("admin-home")).toBeInTheDocument();
    });    
    
    test("Home page error when unable to fetch data", async () => {
        const history = createMemoryHistory();
        history.push("/Home");

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
            <Router history={history}>
                <Home /> 
            </Router>
        );
        
        await waitForSpinner();               
        
        await waitFor(() => expect(screen.getByText("Error Loading Application Data")));
    });    
});
