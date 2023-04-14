// See the 'COPYING' file in the project root for licensing information.
export const Checkbox = ({ id, label, value, description }) => {
    return (
        <div>
            <label htmlFor={id}>{ label }</label>
            <input type="checkbox" checked={value} />
            <div>
                { description && (
                    <div>
                        <span>{ description }</span>
                    </div>
                )}
            </div>
        </div>        
    );
};

export default Checkbox;
