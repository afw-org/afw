// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {ThemeProvider, createTheme} from "@mui/material/styles";

import {AdaptiveProvider} from "@afw/react";
import {AfwClient} from "@afw/client";
import {componentRegistry} from "@afw/react-material-ui";
import {MonacoProvider} from "@afw/react-monaco";

import {AppCoreProvider} from "./App/AppCoreProvider";
import {AppContext} from "./context";

import {rest} from "msw";
import {server} from "@afw/test/build/cjs/__mocks__/server";
import {mswPostCallback, mswGetCallback} from "@afw/test/build/cjs/__mocks__/handlers";
import {render, waitForElementToBeRemoved, userEvent} from "@afw/test";


// mock MU components that use Fade (Collapse/Tree) based on react-transition-group 
jest.mock("@mui/material", () => {
    const materialUI = jest.requireActual("@mui/material");
    return {
        ...materialUI,
        Fade: jest.fn(({ children }) => children),
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
export { rest, server, userEvent, mswPostCallback, mswGetCallback };
