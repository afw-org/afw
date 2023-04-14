// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    Menu
} from "@afw/react";

import {useApplication} from "../hooks";
import {useFlags} from "@afw/react";

export const FlagsSelector = (props) => {

    const [menuTarget, setMenuTarget] = useState();
    const [menuItems, setMenuItems] = useState([]);

    const {flags} = useFlags();
    const {isMobile} = useApplication();

    const {selectedFlags, onSelectFlags} = props;

    const onSelectFlag = (flag) => {
        const flagId = flag.flagId;

        if (selectedFlags && selectedFlags.includes(flagId)) {

            const flags = selectedFlags.filter(selectedFlag =>
                selectedFlag !== flagId
            );
            onSelectFlags(flags);
        } else if (selectedFlags) {
            const flags = [...selectedFlags, flagId];
            onSelectFlags(flags);
        } else {
            const flags = [flagId];
            onSelectFlags(flags);
        }

        setMenuTarget();
    };

    useEffect(() => {

        const isChecked = (flag) => {
            let checked, disabled;
    
            if (selectedFlags) {
                const {flagId, includedByFlags} = flag;

                if (selectedFlags.includes(flagId)) {
                    checked = true;
                } else if (includedByFlags) {
                    
                    for (const includedByFlag of includedByFlags) {
                        if (selectedFlags.includes(includedByFlag)) {
                            checked = disabled = true;
                            break;
                        }
                    }
                }
            }
    
            return { checked, disabled };
        };

        if (flags) {
            setMenuItems(
                flags.map(flag => {
                    const {checked, disabled} = isChecked(flag);

                    return ({
                        key: flag.flagId,
                        label: flag.flagId,
                        description: flag.description,
                        canCheck: true,
                        checked: checked,
                        disabled: disabled,
                        onClick: () => onSelectFlag(flag)
                    });
                })
            );
        }
        // eslint-disable-next-line
    }, [flags, selectedFlags]);

    return (
        <>
            <Button 
                type={isMobile ? "icon": undefined}
                label="Flags"
                variant="text"
                icon="outlined_flag"
                tooltip="Select the execution flags."
                dropdown={true}
                onClick={(event) => setMenuTarget(event.target)}   
            />
            <Menu 
                open={Boolean(menuTarget)}
                target={menuTarget}
                items={menuItems}
                onClose={() => setMenuTarget()}
            />
        </>
    );  
};

export default FlagsSelector;
