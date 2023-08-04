// See the 'COPYING' file in the project root for licensing information.
export const ChoiceGroup = (props) => {

    const {
        label, 
        onChanged, 
        selectedKey, 
        options,
        disabled,
        description,
        ...rest
    } = props;

    return (
        <div {...rest}>
            {
                label && 
                    <p>{ label }</p>
            }
            {
                options && options.map((option) =>
                    <div key={option.key}>
                        <input 
                            type="radio"                             
                            value={option.value} 
                            checked={(selectedKey === option.key)}
                            onChange={e => {
                                if (onChanged)
                                    onChanged(e.currentTarget.value);
                            }}
                            disabled={disabled}
                        />
                        <label for={option.value}>{option.label}</label>
                    </div>
                )
            }
            {
                description && 
                    <span>{ description }</span>
            }
        </div>
    );
};


export default ChoiceGroup;
