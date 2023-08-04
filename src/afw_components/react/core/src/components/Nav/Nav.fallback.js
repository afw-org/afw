// See the 'COPYING' file in the project root for licensing information.
export const Nav = ({ items }) => {

    return (
        <div>
            {
                items && items.map((item, index) =>
                    <div key={index}>
                        <a href={item.url}>{ item.label }</a>
                    </div>
                )
            }
        </div>
    );
};

export default Nav;
