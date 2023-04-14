// See the 'COPYING' file in the project root for licensing information.
import {server, rest, render, waitFor, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";
import Fiddle from "./Fiddle";

describe("Fiddle Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Fiddle renders", async () => {

        render( <Fiddle /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     
        await waitForSpinner();                   

        await waitFor(() => expect(screen.getByTestId("admin-tools-fiddle")).toBeInTheDocument());
    });

    test("Create new fiddle input, evaluate it", async () => {        

        render( <Fiddle /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();    

        await waitFor(() => expect(screen.getByTestId("admin-tools-fiddle")).toBeInTheDocument());

        await waitFor(() => expect(screen.getByLabelText("New Source Window")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("New Source Window"));

        await waitFor(() => expect(screen.getByRole("textbox")).toBeInTheDocument());
        fireEvent.change(screen.getByRole("textbox"), { target: { value: "1 + 1" } });

        mswPostCallback.mockClear();

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                mswPostCallback("/afw", req, res, ctx);
                return res(
                    ctx.status(200),
                    ctx.set("Content-Type", "application/x-afw"),
                    ctx.body("1 31 response\n{\"result\":2,\"status\":\"success\"}2 0 end\n")
                );
            })
        );

        await waitFor(() => expect(screen.getByLabelText("Evaluate")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Evaluate"));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await waitFor(() => expect(screen.getByRole("region")).toBeInTheDocument());


    });

    test("Create new fiddle input, show listing", async () => {

        render( <Fiddle /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());      
        await waitForSpinner();

        await waitFor(() => expect(screen.getByTestId("admin-tools-fiddle")).toBeInTheDocument());

        await waitFor(() => expect(screen.getByLabelText("New Source Window")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("New Source Window"));

        await waitFor(() => expect(screen.getByRole("textbox")).toBeInTheDocument());
        fireEvent.change(screen.getByRole("textbox"), { target: { value: "1 + 1" } });

        mswPostCallback.mockClear();

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                mswPostCallback("/afw", req, res, ctx);
                return res(
                    ctx.status(200),
                    ctx.set("Content-Type", "application/x-afw"),
                    ctx.body("1 722 response\n{\"result\":\"\\n\\n#\\n#  Compiled Adaptive Value Listing\\n#\\n#  Source location: script\\n#\\n---CompiledValue e02a7d14-d4f1-4b57-b00d-60a303950402\\n\\n    1 | 1+1\\n      + compiled_value: [\\n0-0   +     block number=0: [\\n0-3   +         call: [\\n      +             function_definition return\\n0-3   +             call: [\\n      +                 function_definition add\\n      +                 evaluated_integer 1\\n      +                 evaluated_integer 1\\n      +             ]\\n      +         ]\\n      +     ]\\n      + ]\\n\\n\\n---Symbols e02a7d14-d4f1-4b57-b00d-60a303950402\\n\\nblock=0 depth=0 entries=0 : [\\n]\\n\\n\\n\\n---Literals e02a7d14-d4f1-4b57-b00d-60a303950402\\nLiterals coming soon\\n\\n\\n---\\n\",\"status\":\"success\"}2 0 end"),                    
                );
            })
        );

        await waitFor(() => expect(screen.getByLabelText("Listing")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Listing"));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await waitFor(() => expect(screen.getByRole("region")).toBeInTheDocument());


    });

    test("Create new fiddle input, set trace flag, execute", async () => {

        render( <Fiddle /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();

        await waitFor(() => expect(screen.getByTestId("admin-tools-fiddle")).toBeInTheDocument());

        await waitFor(() => expect(screen.getByLabelText("New Source Window")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Flags")).toBeInTheDocument());

        fireEvent.click(screen.getByLabelText("New Source Window"));

        await waitFor(() => expect(screen.getByRole("textbox")).toBeInTheDocument());
        fireEvent.change(screen.getByRole("textbox"), { target: { value: "1 + 1" } });

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Flags"));
        const traceFlag = await screen.findByRole("menuitem", { name: /This produces all basic traces/i });        
        fireEvent.click(traceFlag);

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                mswPostCallback("/afw", req, res, ctx);
                return res(
                    ctx.status(200),
                    ctx.set("Content-Type", "application/x-afw"),
                    ctx.body("1 31 response\n{\"result\":2,\"status\":\"success\"}2 0 end\n")
                );
            })
        );

        await waitFor(() => expect(screen.getByLabelText("Evaluate")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Evaluate"));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("_flags_",
            [
                "response:error",
                "response:stdout",
                "response:stderr",
                "response:stdout:stream",
                "response:stderr:stream",
                "response:console:stream",
                "trace"
            ]
        ));
        await waitForSpinner();

        await waitFor(() => expect(screen.getByRole("region")).toBeInTheDocument());

    });

});
