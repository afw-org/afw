// See the 'COPYING' file in the project root for licensing information.
export const Tooltip = ({ content, target }) => {

    return (
        <div title={content}>
            { target }
        </div>
    );
};

export default Tooltip;
