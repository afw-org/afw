// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-unused-vars */
export const Toolbar = () => (props) => {

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
