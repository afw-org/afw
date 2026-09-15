// See the 'COPYING' file in the project root for licensing information.
 
export const Toolbar = () => function Toolbar(props) {

    /* eslint @typescript-eslint/no-unused-vars: [2, {"varsIgnorePattern": "dense|spacing"}] -- stripped out of ...rest below, not otherwise used by this fallback */
    const {
        dense,
        spacing,
        alignItems,
        justify,
        contains,
        ...rest
    } = props;

    return (
        <div 
            style={{ 
                display: "flex", 
                alignItems, 
                justifyContent: justify,                 
                ...rest 
            }}
        >
            {
                contains && contains
            }
        </div>
    );
};

export default Toolbar;
