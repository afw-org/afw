// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {MemoryRouter} from "react-router-dom";

import {muiTheme} from "storybook-addon-material-ui";

import {AdaptiveProvider} from "@afw/react";

export const decorators = [
    muiTheme(),
    storyFn => {
        return (
            <MemoryRouter>
                <AdaptiveProvider
                    componentRegistry={{ components: {} }}
                >
                    { storyFn() }
                </AdaptiveProvider>
            </MemoryRouter>
        );
    }
];
