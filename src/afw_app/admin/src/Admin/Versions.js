// See the 'COPYING' file in the project root for licensing information.

import Container from "../common/Container";
import {
    Divider,
    ObjectResponsive,
    Paper,
    Spinner,
    Typography,
    useVersionInfo
} from "@afw/react";

import {useTheme} from "../hooks";

export const Versions = () => {

    const {versions, isLoading} = useVersionInfo();
    const theme = useTheme();

    return (
        <Container>
            <Typography size="8" text="Version Information" />
            <div style={{ height: theme.spacing(5) }} />
            <Typography 
                size="3" 
                text="The core, along with every extension and compiled application, has associated version information.
                    Version information draws upon both manually maintained semantic versions and source control derived 
                    version information."
            />
            <div style={{ height: theme.spacing(5) }} />
            {
                isLoading &&
                    <Spinner size="large" label="Loading Version Information..." fullScreen={true} />
            }
            {
                versions && versions.map(version => 
                    <div key={version.getObjectId()} data-testid={version.getObjectId()}>
                        <Paper 
                            contains={
                                <ObjectResponsive 
                                    style={{ padding: theme.spacing(2) }}
                                    headerOptions="all"
                                    onRenderHeader={() => 
                                        <div>
                                            <Typography size="6" text={version.getObjectId()} />
                                            <div style={{ height: theme.spacing(1) }} />
                                            <Divider />
                                            <div style={{ height: theme.spacing(1) }} />
                                        </div>
                                    }
                                    object={version}
                                    spacing="0px"
                                    preferredComponents={[
                                        {
                                            componentProps: {
                                                tooltip: true
                                            }
                                        }
                                    ]}
                                />
                            }
                        />
                        <div style={{ height: theme.spacing(5) }} />
                    </div>
                )
            }
        </Container>
    );
};

export default Versions;
