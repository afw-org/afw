// See the 'COPYING' file in the project root for licensing information.
import {AdaptiveComponent} from "../AdaptiveComponent";

export const List = ({ contains }) => {
    return (
        <ul>
            {
                contains && contains.map((c, index) => 
                    <li key={index}>
                        <AdaptiveComponent layoutComponent={c} />                            
                    </li>
                )
            }
        </ul>
    );
};

export default List;
