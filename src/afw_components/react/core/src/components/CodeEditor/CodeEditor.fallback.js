// See the 'COPYING' file in the project root for licensing information.
export const CodeEditor = ({ id, "data-testid": dataTestId, label, source, onChanged }) => 
    <div role="code">
        <label htmlFor={id}>{label}</label>
        <textarea 
            id={id} 
            aria-label={label}
            data-testid={dataTestId} 
            value={source}             
            onChange={e => onChanged(e.target.value)}></textarea>
    </div>;

export default CodeEditor;
