// See the 'COPYING' file in the project root for licensing information.
import React, {forwardRef} from "react";

import {muiTheme} from "storybook-addon-material-ui";

import {AdaptiveProvider} from "@afw/react";

/*
 * Keep link clicks inside the story: a router-free navigation adapter (see
 * @afw/react's navigation.js) whose links don't leave the page.
 */
const StoryLink = forwardRef(({ to, onClick, children, ...rest }, ref) =>
    <a ref={ref} {...rest} href={to} onClick={(event) => {
        if (onClick)
            onClick(event);
        event.preventDefault();
    }}>{children}</a>
);
StoryLink.displayName = "StoryLink";

const storyNavigation = {
    Link: StoryLink,
    useNavigate: () => () => undefined,
    NavigationBlocker: () => null,
};

export const decorators = [
    muiTheme(),
    storyFn => {
        return (
            <AdaptiveProvider
                componentRegistry={{ components: {} }}
                navigation={storyNavigation}
            >
                { storyFn() }
            </AdaptiveProvider>
        );
    }
];
