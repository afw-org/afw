// See the 'COPYING' file in the project root for licensing information.
module.exports = {
    "stories": [
        "../react/core/src/**/*.stories.mdx",
        "../react/core/src/**/*.stories.@(js|jsx|ts|tsx)",
        "../react/material/src/**/*.stories.mdx",
        "../react/material/src/**/*.stories.@(js|jsx|ts|tsx)"
    ],
    "addons": [
        "@storybook/addon-links",
        "@storybook/addon-essentials",
        "@storybook/preset-create-react-app"
    ],
    webpackFinal: config => {
        return {
            ...config,
            plugins: config.plugins.filter(plugin => {
                if (plugin.constructor.name === 'ESLintWebpackPlugin') {
                    return false
                }
                return true
            }),
        }
    }
}
