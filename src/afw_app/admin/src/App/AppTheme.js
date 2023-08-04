// See the 'COPYING' file in the project root for licensing information.
import useMediaQuery from "@mui/material/useMediaQuery";

/**
 * Global Application Theme Settings.
 */

export const lightTheme = {
    palette: {
        mode: "light",
        primary: {
            light: "#deecf9",
            main: "#005a9e",
            dark: "#004578",
            contrastText: "#fff",
        },
        secondary: {
            light: "#00bcf2",
            main: "#0078d4",
            dark: "#002050",
            contrastText: "#000",
        },
        background: {
            paper: "#fafafa",
            default: "#fff"
        }
    },
    components: {
        MuiPaper: {
            styleOverrides: { root: { backgroundImage: "unset" }}
        },
    },   
    mixins: {
        denseToolbar: {
            minHeight: 48
        }
    } 
};

export const darkTheme = {
    palette: {
        mode: "dark",
        primary: {
            light: "#deecf9",
            main: "#90caf9",
            dark: "#648dae",
            contrastText: "#000",
        },
        secondary: {
            light: "#00bcf2",
            main: "#0078d4",
            dark: "#002050",
            contrastText: "#000",
        },
        background: {
            paper: "#242424",
            default: "#151515",
        },
    },
    components: {
        MuiPaper: {
            styleOverrides: { root: { backgroundImage: "unset" }}
        },
    },
    mixins: {
        denseToolbar: {
            minHeight: 48
        }
    }
};

/**
 * 
 * A HOC to wrap the useMediaQuery hook in order to get preferred settings.
 */
export const withMediaQuery = (Component) => {
    return (props) => {
        const prefersDarkMode = useMediaQuery("(prefers-color-scheme: dark)");
        const prefersHighContrast = useMediaQuery("(prefers-contrast: high)");
        const prefersReducedMotion = useMediaQuery("(prefers-reduced-motion: reduce)");
        const prefersReducedTransparency = useMediaQuery("(prefers-reduced-transparency: reduce)");

        return (
            <Component 
                prefersDarkMode={prefersDarkMode} 
                prefersHighContrast={prefersHighContrast}
                prefersReducedMotion={prefersReducedMotion}
                prefersReducedTransparency={prefersReducedTransparency}
                {...props} 
            />
        );
    };
};
