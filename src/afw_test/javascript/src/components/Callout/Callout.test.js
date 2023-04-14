// See the 'COPYING' file in the project root for licensing information.
import React, {useState} from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const CalloutTest = ({ Callout }) => {
    const [anchorEl, setAnchorEl] = useState(null);
    
    return (
        <>
            <button 
                onClick={e => setAnchorEl(e.currentTarget) } 
            />                
            <Callout
                open={Boolean(anchorEl)}  
                target={anchorEl}         
                contains={
                    <span>This is inside a Callout!</span>
                }
            />
        </>
    );
};

const Test = (wrapper, Callout) => {

    test("Renders properly, text not visible when closed", async () => {                        

        render(<CalloutTest Callout={Callout} />, { wrapper });        

        expect(screen.queryByText(/This is inside a Callout/)).not.toBeInTheDocument();
    });

    test("Renders properly, text visible after open", async () => {           

        render(<CalloutTest Callout={Callout} />, { wrapper });

        fireEvent.click(screen.getByRole("button"));
        
        expect(screen.getByText(/This is inside a Callout!/)).toBeInTheDocument();
    });
};

export default Test;
