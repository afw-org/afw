// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    Checkbox,
    Dialog,
    Dropdown,    
    Responsive,
    Slider,
    Tabs,
    TextField,
    Typography,
} from "@afw/react";

import FlagsSelector from "../common/FlagsSelector";
import {lightTheme, darkTheme} from "./AppTheme";
import {useApplication, useLocalJSONStorage, useTheme} from "../hooks";

const appDefaultSettings = {
    timeout: 30000,
};

/**
 * ColorPicker
 * 
 * Displays a TextField along with a "preview" of the color that has been chosen.
 * We could get really fancy here in the future and use react-color or something to
 * actually choose from a palette by clicking.
 */
const ColorPicker = ({ label, defaultValue, onChanged }) => {

    const theme = useTheme();
    const [value, setValue] = useState();
    const [, setPickColor] = useState();

    const onColorChanged = (newValue) => {
        setValue(newValue);
    };

    useEffect(() => {
        /* when a value is valid, notify main settings app component */
        if (value && value.match(/^#[0-9A-Fa-f]{6}$/)) {
            onChanged(value);
        }

        // eslint-disable-next-line
    }, [value]);

    return (
        <div style={{ display: "flex", alignItems: "center", marginBottom: theme.spacing(1) }}>
            <div 
                role="button"
                tabIndex="0"
                onKeyPress={() => setPickColor(true)}
                onClick={() => setPickColor(true)}
                style={{ 
                    marginRight: theme.spacing(1), 
                    width: theme.spacing(3), 
                    height: theme.spacing(3), 
                    backgroundColor: value ? value : defaultValue, border: "solid thin",
                    cursor: "pointer",
                }} 
            />
            <div style={{ flex: 1 }}>
                <TextField 
                    style={{ verticalAlign: "bottom", color: value ? value : defaultValue }}
                    label={label}
                    value={value ? value : defaultValue}
                    onChanged={onColorChanged}
                />
            </div>
        </div>
    );
};

/*
 * AppSettings
 *
 * This component defines the general Settings for the App
 * and allows the user to change them.  This includes UI
 * framework, theme palette, fonts, caching, timeouts, etc.
 */
const AppSettings = (props) => {    
    
    const {open, onDismiss} = props;    
    const {client, theme, onChanged} = useApplication();
    const [selectedFlags, setSelectedFlags] = useState([]);
    const [selectedTheme, setSelectedTheme] = useState();
    const [originalTheme, setOriginalTheme] = useState();
    const {storage, onChanged: onStorageChanged} = useLocalJSONStorage("settings", appDefaultSettings);
    const [save, setSave] = useState(true);
    const [palettePrimary, setPalettePrimary] = useState();
    const [paletteSecondary, setPaletteSecondary] = useState();
    const [paletteError, setPaletteError] = useState();
    const [paletteBackground, setPaletteBackground] = useState();
    const [timeout, setTimeout] = useState(30000);    

    useEffect(() => {
        if (storage && storage.flags) 
            setSelectedFlags(storage.flags);
        if (storage && storage.timeout)
            setTimeout(storage.timeout);    
        
    }, [storage]);
   
    const onApply = () => {

        if (selectedFlags)
            client.setActionFlags(selectedFlags);
        
        if (timeout)
            client.setTimeout(timeout);

        let newTheme = theme;
        if (palettePrimary) {
            newTheme = { ...newTheme, palette: {
                ...newTheme.palette,
                primary: {
                    ...newTheme.palette.primary,
                    ...palettePrimary
                }
            }};
        }

        if (paletteSecondary) {
            newTheme = { ...newTheme, palette: {
                ...newTheme.palette,
                secondary: {
                    ...newTheme.palette.secondary,
                    ...paletteSecondary
                }
            }};
        }

        if (paletteError) {
            newTheme = { ...newTheme, palette: {
                ...newTheme.palette,
                error: {
                    ...newTheme.palette.error,
                    ...paletteError
                }
            }};
        }

        if (paletteBackground) {
            newTheme = { ...newTheme, palette: {
                ...newTheme.palette,
                background: {
                    ...newTheme.palette.background,
                    ...paletteBackground
                }
            }};
        }

        /* if any of our colors were changed, apply them */
        if (palettePrimary || paletteSecondary || paletteError || paletteBackground) {
            onChanged({ theme: newTheme });
        }

        if (save) {
            let updatedStorage = storage;
            if (selectedFlags)
                updatedStorage.flags = selectedFlags;
            if (timeout)
                updatedStorage.timeout = timeout;            
            if (selectedTheme)
                updatedStorage.theme = selectedTheme;
            if (palettePrimary || paletteSecondary || paletteError || paletteBackground) {
                updatedStorage.theme = {
                    type: newTheme.type,
                    palette: newTheme.palette,
                };
            }

            onStorageChanged(updatedStorage);
        }

        props.onDismiss();        
    };

    const onCancel = () => {
        /* revert any UI changes, if made */            
        if (originalTheme)
            onChanged({ theme: originalTheme });            

        props.onDismiss();
    };

    return (
        <Dialog 
            open={open}
            onDismiss={onDismiss}
            title="Web App Settings"             
            maxWidth="xl"   
            showClose={true}
            contains={
                <div style={{ height: "70vh", overflow: "auto" }}>
                    <Tabs 
                        tabs={[
                            {
                                text: "Look and Feel",
                                contains:
                                    <div style={{ padding: theme.spacing(2) }}>  
                                        <div style={{ paddingTop: theme.spacing(5) }}>
                                            <Typography size="7" text="Theme Settings" />
                                            <div style={{ height: theme.spacing(1) }} />
                                            <div>
                                                <Tabs 
                                                    gapSpace={24}
                                                    tabs={[                                        
                                                        {
                                                            text: "Palette",
                                                            contains: 
                                                                <>
                                                                    <div style={{ width: "400px" }}>
                                                                        <Dropdown 
                                                                            label="Theme Type"
                                                                            options={[
                                                                                {
                                                                                    key: "light",
                                                                                    text: "Light"
                                                                                },
                                                                                {
                                                                                    key: "dark",
                                                                                    text: "Dark"
                                                                                },
                                                                            ]}
                                                                            selectedKey={theme.palette.mode}
                                                                            onChanged={option => {                                                            
                                                                                onChanged({
                                                                                    theme: (option.key === "dark") ? darkTheme : lightTheme
                                                                                });
                                                                                if (!originalTheme)
                                                                                    setOriginalTheme(theme);

                                                                                setSelectedTheme({
                                                                                    ...selectedTheme,
                                                                                    mode: option.key,
                                                                                });
                                                                            }}
                                                                        />                                                                    
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(5) }} />
                                                                    <Responsive 
                                                                        spacing={8}
                                                                        items={[
                                                                            
                                                                            {
                                                                                breakpoints: {
                                                                                    small: 12,
                                                                                    medium: 4,
                                                                                    large: 3,
                                                                                    xl: 3,
                                                                                },
                                                                                contains: 
                                                                                    <div>
                                                                                        <Typography size="6" text="Primary Colors" />
                                                                                        <div style={{ height: theme.spacing(2) }} />
                                                                                        <ColorPicker 
                                                                                            label="Light"
                                                                                            defaultValue={theme.palette.primary.light}       
                                                                                            onChanged={light => setPalettePrimary({...palettePrimary, light })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Main"
                                                                                            defaultValue={theme.palette.primary.main}     
                                                                                            onChanged={main => setPalettePrimary({...palettePrimary, main })}                                                                                               
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Dark"
                                                                                            defaultValue={theme.palette.primary.dark}   
                                                                                            onChanged={dark => setPalettePrimary({...palettePrimary, dark })}                                                                                                 
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Contrast"
                                                                                            defaultValue={theme.palette.primary.contrastText}
                                                                                            onChanged={contrastText => setPalettePrimary({...palettePrimary, contrastText })}                                                                                                  
                                                                                        />                                                                                                
                                                                                    </div>
                                                                            },
                                                                            {
                                                                                breakpoints: {
                                                                                    small: 12,
                                                                                    medium: 4,
                                                                                    large: 3,
                                                                                    xl: 3,
                                                                                },
                                                                                contains: 
                                                                                    <div>
                                                                                        <Typography size="6" text="Secondary Colors" />
                                                                                        <div style={{ height: theme.spacing(2) }} />
                                                                                        <ColorPicker 
                                                                                            label="Light"
                                                                                            defaultValue={theme.palette.secondary.light}
                                                                                            onChanged={light => setPaletteSecondary({...paletteSecondary, light })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Main"
                                                                                            defaultValue={theme.palette.secondary.main}
                                                                                            onChanged={main => setPaletteSecondary({...paletteSecondary, main })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Dark"
                                                                                            defaultValue={theme.palette.secondary.dark}
                                                                                            onChanged={dark => setPaletteSecondary({...paletteSecondary, dark })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Contrast"
                                                                                            defaultValue={theme.palette.secondary.contrastText}
                                                                                            onChanged={contrastText => setPaletteSecondary({...paletteSecondary, contrastText })}
                                                                                        />
                                                                                    </div>
                                                                            },
                                                                            {
                                                                                breakpoints: {
                                                                                    small: 12,
                                                                                    medium: 4,
                                                                                    large: 3,
                                                                                    xl: 3
                                                                                },
                                                                                contains: 
                                                                                    <div>
                                                                                        <Typography size="6" text="Error Colors" />
                                                                                        <div style={{ height: theme.spacing(2) }} />
                                                                                        <ColorPicker 
                                                                                            label="Light"
                                                                                            defaultValue={theme.palette.error.light}
                                                                                            onChanged={light => setPaletteError({...paletteError, light })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Main"
                                                                                            defaultValue={theme.palette.error.main}
                                                                                            onChanged={main => setPaletteError({...paletteError, main })}                                                                                             
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Dark"
                                                                                            defaultValue={theme.palette.error.dark}
                                                                                            onChanged={dark => setPaletteError({...paletteError, dark })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Contrast"
                                                                                            defaultValue={theme.palette.error.contrastText}
                                                                                            onChanged={contrastText => setPaletteError({...paletteError, contrastText })}
                                                                                        />
                                                                                    </div>
                                                                            },
                                                                            {
                                                                                breakpoints: {
                                                                                    small: 12,
                                                                                    medium: 4,
                                                                                    large: 3,
                                                                                    xl: 3
                                                                                },
                                                                                contains: 
                                                                                    <div>
                                                                                        <Typography size="6" text="Background Colors" />
                                                                                        <div style={{ height: theme.spacing(2) }} />
                                                                                        <ColorPicker 
                                                                                            label="Paper"
                                                                                            defaultValue={theme.palette.background.paper}
                                                                                            onChanged={paper => setPaletteBackground({...paletteBackground, paper })}
                                                                                        />
                                                                                        <ColorPicker 
                                                                                            label="Default"
                                                                                            defaultValue={theme.palette.background.default}
                                                                                            onChanged={d => setPaletteBackground({...paletteBackground, default: d })}
                                                                                        />                                                                                                
                                                                                    </div>
                                                                            },
                                                                        ]}
                                                                    />     
                                                                    <div style={{ height: theme.spacing(4) }} />
                                                                    <div style={{ width: "300px" }}>
                                                                        <TextField 
                                                                            label="Contrast Threshold"
                                                                            value={theme.palette.contrastThreshold}
                                                                        />               
                                                                        <TextField 
                                                                            label="Tonal Offset"
                                                                            value={theme.palette.tonalOffset}
                                                                        />   
                                                                    </div>                                             
                                                                </>
                                                        },
                                                        {
                                                            text: "Breakpoints",
                                                            contains: 
                                                                <div>     
                                                                    <div style={{ display: "flex", alignItems: "center" }}>
                                                                        <div style={{ flex: 1, marginRight: theme.spacing(1) }}>
                                                                            <Slider 
                                                                                label="Extra Small Min Width"
                                                                                min={0}
                                                                                max={1920}
                                                                                step={8}
                                                                                value={theme.breakpoints.values.xs}
                                                                            />  
                                                                        </div>
                                                                        <Typography 
                                                                            text={String(theme.breakpoints.values.xs)} 
                                                                        />
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(3) }} />
                                                                    <div style={{ display: "flex", alignItems: "center" }}>
                                                                        <div style={{ flex: 1, marginRight: theme.spacing(1) }}>
                                                                            <Slider 
                                                                                label="Small Min Width"
                                                                                min={0}
                                                                                max={1920}
                                                                                step={8}
                                                                                value={theme.breakpoints.values.sm}
                                                                            />  
                                                                        </div>
                                                                        <Typography 
                                                                            text={String(theme.breakpoints.values.sm)} 
                                                                        />
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(3) }} />
                                                                    <div style={{ display: "flex", alignItems: "center" }}>
                                                                        <div style={{ flex: 1, marginRight: theme.spacing(1) }}>
                                                                            <Slider 
                                                                                label="Medium Min Width"
                                                                                min={0}
                                                                                max={1920}
                                                                                step={8}
                                                                                value={theme.breakpoints.values.md}
                                                                            />  
                                                                        </div>
                                                                        <Typography 
                                                                            text={theme.breakpoints.values.md} 
                                                                        />
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(3) }} />
                                                                    <div style={{ display: "flex", alignItems: "center" }}>
                                                                        <div style={{ flex: 1, marginRight: theme.spacing(1) }}>
                                                                            <Slider 
                                                                                label="Large Min Width"
                                                                                min={0}
                                                                                max={1920}
                                                                                step={8}
                                                                                value={theme.breakpoints.values.lg}
                                                                            />  
                                                                        </div>
                                                                        <Typography 
                                                                            text={theme.breakpoints.values.lg} 
                                                                        />
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(3) }} />
                                                                    <div style={{ display: "flex", alignItems: "center" }}>
                                                                        <div style={{ flex: 1, marginRight: theme.spacing(1) }}>
                                                                            <Slider 
                                                                                label="Extra Large Min Width"
                                                                                min={0}
                                                                                max={1920}
                                                                                step={8}
                                                                                value={theme.breakpoints.values.xl}
                                                                            />  
                                                                        </div>
                                                                        <Typography 
                                                                            text={theme.breakpoints.values.xl} 
                                                                        />
                                                                    </div>
                                                                    <div style={{ height: theme.spacing(3) }} />                                                                    
                                                                </div>
                                                        },
                                                        {
                                                            text: "Typography",
                                                            contains: 
                                                                <div style={{ width: "400px" }}>                                                                    
                                                                    <TextField 
                                                                        label="Font Family"
                                                                        value={theme.typography.fontFamily}
                                                                    />
                                                                    <TextField 
                                                                        label="Font Size"
                                                                        value={theme.typography.fontSize}
                                                                    />
                                                                    <TextField 
                                                                        label="HTML Font Size"
                                                                        value={theme.typography.htmlFontSize}
                                                                    />
                                                                    <TextField 
                                                                        label="Font Weight Light"
                                                                        value={theme.typography.fontWeightLight}
                                                                    />
                                                                    <TextField 
                                                                        label="Font Weight Regular"
                                                                        value={theme.typography.fontWeightRegular}
                                                                    />
                                                                    <TextField 
                                                                        label="Font Weight Medium"
                                                                        value={theme.typography.fontWeightMedium}
                                                                    />
                                                                    <TextField 
                                                                        label="Font Weight Bold"
                                                                        value={theme.typography.fontWeightBold}
                                                                    />
                                                                </div>
                                                        }
                                                    ]}
                                                />
                                            </div>
                                        </div>
                                    </div>  
                            },
                            {
                                text: "Session",
                                contains: 
                                    <div style={{ padding: theme.spacing(2) }}>
                                        <div style={{ width: "400px" }}>
                                            <TextField 
                                                label="Network Timeout"
                                                value={timeout}
                                                onChanged={setTimeout}
                                                description="The network timeout, in milliseconds, on requests."
                                            />
                                        </div>
                                        <div style={{ height: theme.spacing(5) }} />
                                        <div>
                                            <Typography color="textSecondary" size="2" text="Default Session Flags" />
                                            <FlagsSelector 
                                                selectedFlags={selectedFlags}
                                                onSelectFlags={setSelectedFlags}
                                            />
                                            <ul style={{ listStyle: "none" }}>
                                                {
                                                    selectedFlags.map(flag => <li key={flag}>{ flag }</li> )
                                                }
                                            </ul>
                                            <Typography color="textSecondary" size="1" text="Set the default flags to be used on the underlying client.  These will be used, in addition to what is defined by the Application." />
                                        </div>
                                    </div>
                            }
                        ]}
                    />  
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>
                    <Checkbox 
                        style={{ display: "inline-block" }}
                        label="Remember Settings between clients."
                        value={save}
                        onChanged={setSave}
                    />
                    <div style={{ display: "flex" }}>
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            label="Cancel"   
                            onClick={onCancel}                         
                        />
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            label="Apply" 
                            color="primary"   
                            variant="contained"
                            onClick={onApply}                         
                        />
                    </div>
                </div>
            }
        />
    );    
};

export default AppSettings;
