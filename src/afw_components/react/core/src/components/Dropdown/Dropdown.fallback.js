// See the 'COPYING' file in the project root for licensing information.
export const Dropdown = ({ id, label, description, options, selectedKeys }) => {

    return (
        <div>
            <label htmlFor={id}>{ label }</label>
            <div>
                <select id={id}>
                    {
                        options && options.map(option => 
                            <option 
                                key={option.key} 
                                value={option.text}
                                selected={selectedKeys && selectedKeys.includes(option.key)}
                            >{option.text}</option>
                        )
                    }
                </select>
            </div>            
            {
                description && (
                    <div>
                        <span>{ description }</span>
                    </div>
                )
            }
        </div>        
    );
};

export default Dropdown;
