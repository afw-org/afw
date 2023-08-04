// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

export const Collapsible = (props) => {

    const {isCollapsed, setIsCollapsed} = useState(props.isCollapsed);

    const {
        contains,
        headerItems,
    } = props;

    const onToggle = () => {
        setIsCollapsed(is => !is);
    };

    return (
        <>
            <div style={{ display: "flex" }}>
                <div style={{ flex: 1 }}>
                    {
                        headerItems && 
                            headerItems
                    }
                </div>
                <button                     
                    onClick={onToggle}
                />
            </div>
            {
                !isCollapsed &&
                    contains
            }
        </>
    );
};

export default Collapsible;
