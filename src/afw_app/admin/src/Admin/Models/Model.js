// See the 'COPYING' file in the project root for licensing information.
import ModelOverview from "./Overview/ModelOverview";
import ModelMapping from "./Mappings/ModelMapping";
import ModelTree from "./Tree/ModelTree";
import ModelSpreadsheet from "./Spreadsheet/ModelSpreadsheet";
import ModelSource from "./Source/ModelSource";

/*
 * Model
 *
 * This component represents the top-level of the Model hierarchy.
 * It allows the user to navigate the Object Types and General 
 * Model properties.
 */
export const Model = (props) => {
  
    const {perspective, model} = props;    

    const {objectTypes} = model.getPropertyValues();    
   
    if (!model)
        return null;    

    /* Based on the perspective, chosen by url and/or clicking the button, 
       return the appropriate component */
    switch (perspective) {

    case "tree":
        return <ModelTree {...props} />;

    case "spreadsheet":
        return <ModelSpreadsheet {...props} />;    

    case "mappings":
        return <ModelMapping {...props} modelObjectTypeObjects={objectTypes} />;

    case "source":
        return <ModelSource {...props} />;        

    case "overview":
    default:
        return <ModelOverview {...props} />;
    }
    
};

export default Model;
