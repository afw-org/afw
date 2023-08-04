// See the 'COPYING' file in the project root for licensing information.
export const TimezonePicker = () => ({ id, label, description, placeholder, value, onChanged }) => {
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
                value={value}
                placeholder={placeholder}
                onChange={e => {
                    if (onChanged)
                        onChanged(e.target.value);
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

export default TimezonePicker;
