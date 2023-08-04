// See the 'COPYING' file in the project root for licensing information.
export const Autocomplete = ({ id, label, description, value, onChanged }) => {
    const textValue = value ? value.text : "";

    return (
        <div>
            { 
                label && (
                    <div>
                        <label htmlFor={id}>{label}</label>
                    </div>
                )
            }
            <input 
                id={id} 
                value={textValue}
                onChange={e => {
                    if (onChanged)
                        onChanged({ key: e.target.value, text: e.target.value });
                }} 
            />
            {
                description &&
                    <div>
                        <span>{description}</span>
                    </div>
            }
        </div>
    );
};

export default Autocomplete;
