// See the 'COPYING' file in the project root for licensing information.
const Box = (props) => {
    const {items, ...rest} = props;

    return (
        <div style={{ display: "flex", ...rest }}>
            { items && items.map((item, index) => <Box.Item {...item} key={index} />) }
        </div>
    );
};

Box.Item = (props) => {

    const {children, ...rest} = props;

    return (
        <div style={{ ...rest }}>
            { children }
        </div>
    );
};

export default Box;
