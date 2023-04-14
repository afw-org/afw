// See the 'COPYING' file in the project root for licensing information.
import {Fragment} from "react";

export const Breadcrumb = ({ items }) => {
    return (
        <>
            {
                items && items.map((item, index) => 
                    <Fragment key={index}>
                        <a href={item.link}>{item.text}</a>
                        <span> / </span>
                    </Fragment>
                )
            }
        </>
    );
};

export default Breadcrumb;
