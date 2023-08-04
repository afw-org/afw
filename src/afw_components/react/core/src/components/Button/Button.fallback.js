// See the 'COPYING' file in the project root for licensing information.
export const Button = ({ label, onClick }) => {
    
    return (
        <button onClick={onClick} >
            { label }
        </button>
    );
};

export default Button;
