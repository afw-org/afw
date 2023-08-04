// See the 'COPYING' file in the project root for licensing information.
import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, Menu) => {

    describe("Menu tests", () => {

        test("Renders properly with 2 menu options", async () => {
                            
            const target = document.createElement("div");
            const {queryByText} = render(                
                <Menu       
                    open={true}
                    target={target}
                    items={[
                        {
                            key: "item1",
                            label: "Item 1",
                        },
                        {
                            key: "2",
                            label: "This is a Menu item!"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Menu item!/)).toBeInTheDocument();               
        });

        test("Renders properly with nested menu options", async () => {
                            
            const target = document.createElement("div");
            render(                
                <Menu       
                    open={true}
                    target={target}
                    items={[
                        {
                            key: "1",
                            label: "Item 1",
                        },
                        {
                            key: "2",
                            label: "Item 2",
                            subMenu: {
                                items: [
                                    {
                                        key: "2a",
                                        label: "Item 2a",
                                    },
                                    {
                                        key: "2b",
                                        label: "Item 2b",
                                    }
                                ]
                            }
                        },
                        {
                            key: "3",
                            label: "Item 3",
                        }
                    ]}
                />,
                { wrapper }
            );        
                        
            expect(screen.queryByRole("menuitem", { name: "Item 2" })).toBeInTheDocument(); 
            expect(screen.queryByRole("menuitem", { name: "Item 2a" })).not.toBeInTheDocument();               
                        
            fireEvent.click(screen.queryByRole("menuitem", { name: "Item 2" }));

            expect(await screen.findByRole("menuitem", { name: "Item 2a" })).toBeInTheDocument();        
        });

        test("Clicking menu items calls onClick", async () => {
                            
            const onClick = jest.fn();
            const target = document.createElement("div");
            render(                
                <Menu       
                    open={true}
                    target={target}
                    items={[
                        {
                            key: "1",
                            label: "Item 1",
                        },
                        {
                            key: "2",
                            label: "Item 2",
                            subMenu: {
                                items: [
                                    {
                                        key: "2a",
                                        label: "Item 2a",
                                        onClick
                                    },
                                    {
                                        key: "2b",
                                        label: "Item 2b",
                                    }
                                ]
                            }
                        },
                        {
                            key: "3",
                            label: "Item 3",
                        }
                    ]}
                />,
                { wrapper }
            );        
                        
            expect(screen.queryByRole("menuitem", { name: "Item 2" })).toBeInTheDocument();                          
                        
            /* expand sub menu */
            fireEvent.click(screen.queryByRole("menuitem", { name: "Item 2" }));

            expect(screen.queryByRole("menuitem", { name: "Item 2a" })).toBeInTheDocument();        

            /* click sub menu item */
            fireEvent.click(screen.queryByRole("menuitem", { name: "Item 2a" }));

            expect(onClick).toHaveBeenCalled();
        });
        
    });
};

export default Test;
