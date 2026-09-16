// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {ThemeProvider, createTheme} from "@mui/material/styles";

import {AdaptiveProvider} from "@afw/react";
import {AfwClient} from "@afw/client";
import {componentRegistry} from "@afw/react-material-ui";
import {MonacoProvider} from "@afw/react-monaco";

import {AppCoreProvider} from "./App/AppCoreProvider";
import {AppContext} from "./context";

import {vi} from "vitest";
// `rest`/`server`/`mswPostCallback`/`mswGetCallback` must come from this same
// "@afw/test" package import (not a separate deep "@afw/test/build/cjs/..."
// path) - setupTests.js's beforeAll(() => server.listen(...)) registers
// handlers on the `server` singleton resolved this way, and a deep-path
// import resolves a *different* build variant (esm vs cjs), landing on a
// distinct module instance with its own unconnected `server`/mock functions,
// so server.use()/mock assertions here would silently talk to a server
// nothing is actually listening on. Named imports of these specifically fail
// once this file (a plain .js, not .jsx) has gone through its own esbuild
// JSX transform (see jsxInJs() in vitest.config.js) - Vite's static
// named-export detection doesn't reliably survive that, so a namespace
// import/destructure is used instead.
import * as afwTest from "@afw/test";

const {http, HttpResponse, server, mswPostCallback, mswGetCallback, render, waitForElementToBeRemoved, userEvent} = afwTest;


// mock MU components that use Fade (Collapse/Tree) based on react-transition-group
vi.mock("@mui/material", async () => {
    const materialUI = await vi.importActual("@mui/material");
    return {
        ...materialUI,
        Fade: vi.fn(({ children }) => children),
    };
});


const AllTheProviders = ({ children }) => {    
    const client = new AfwClient({ url: "/afw" });

    const onHandleAppRequestError = jest.fn();
    const notification = jest.fn();
    const onChanged = jest.fn();
    const onCopy = jest.fn();
    const clipboard = [];

    const applicationProps = {
        client, onHandleAppRequestError, notification, onChanged, onCopy
    };

    return (        
        <ThemeProvider theme={createTheme({})}>
            <AppContext.Provider value={applicationProps}>
                <MemoryRouter>
                    <AdaptiveProvider 
                        client={client}
                        componentRegistry={componentRegistry} 
                        onCopy={onCopy} 
                        clipboard={clipboard} 
                    >
                        <AppCoreProvider>
                            <MonacoProvider theme="dark">
                                { children }
                            </MonacoProvider>                                
                        </AppCoreProvider>                        
                    </AdaptiveProvider>
                </MemoryRouter>
            </AppContext.Provider>
        </ThemeProvider>
    );
};

const customRender = (ui, options) =>
    render(ui, { wrapper: AllTheProviders, ...options });

export const waitForSpinner = async () => {
    if (document.querySelector("[data-component-type='Spinner']"))
        return waitForElementToBeRemoved(() => document.querySelector("[data-component-type='Spinner']"));
};

// re-export everything
export * from "@testing-library/react";
export { customRender as render, AllTheProviders };
export { http, HttpResponse, server, userEvent, mswPostCallback, mswGetCallback };
