/**
 * 
 * Okay, the java ride is going to be later
 * i just need to generate the profile for
 * this device.
 * 
 */

var XML = require("xml2js")
var mustache = require("mustache")
const { program } = require('commander');
var fs = require("fs")
const util = require('util')

program
    .option("-o, --output <directory>")
    .argument("<gsdml>")

program.parse()

const options = program.opts();

var gsdmlParse = XML.parseString;
var gsdmlEncode = (new XML.Builder());

var gsdmlPath = fs.realpathSync(program.args[0]);
var outputPath = options.output


console.log("GSDML FIle: ", gsdmlPath)
console.log("Output Directory: ", outputPath)

try {
    fs.mkdirSync(fs.realpathSync(outputPath))
} catch (e)
{
    console.log(`failed to create directory at ${outputPath}`)
}

if (!fs.existsSync(gsdmlPath))
{
    console.log(`GSDML file at ${gsdmlPath} doesn't exist`)
}

var gsdmlBuffer = fs.readFileSync(gsdmlPath)

gsdmlParse(gsdmlBuffer, (err, result) => {
    
    if (err) {
        console.log(err);
        return;
    }

    // console.log(JSON.stringify(result))
});

var gsdml = require("./models/data").data
var template = require("./models/template").data



function getModules(gsdml)
{
    return gsdml.ISO15745Profile.ProfileBody[0].ApplicationProcess[0].ModuleList[0].ModuleItem;
}

var modules = getModules(gsdml)

function GSDMLSubmoduleToTemplateModel(gsdml, submodule) {

}

function GSDMLModuleToTemplateModel(gsdml, module) {

    var Name = getTextById(gsdml, module.ModuleInfo[0].Name[0].$.TextId).replace(" ", "_")
    var InfoText = getTextById(gsdml, module.ModuleInfo[0].InfoText[0].$.TextId)
    var ID = module.$.ID
    var ModuleIdentNumber = module.$.ModuleIdentNumber
    var VirtualSubmoduleList = module.VirtualSubmoduleList

    var Submodules = VirtualSubmoduleList.map((element) => {
        
        var ID = element.VirtualSubmoduleItem[0].$.ID;
        var IOData = element.VirtualSubmoduleItem[0].IOData;
        var ModuleInfo = element.VirtualSubmoduleItem[0].ModuleInfo;
        var Name = element.VirtualSubmoduleItem[0].ModuleInfo[0].Name[0].$.TextId;
        var InfoText = element.VirtualSubmoduleItem[0].ModuleInfo[0].InfoText[0].$.TextId;
        var RecordDataList = (element.VirtualSubmoduleItem[0].RecordDataList);
        if (RecordDataList !== undefined) RecordDataList = RecordDataList[0];
        var ParameterRecordDataItem = RecordDataList?.ParameterRecordDataItem;

        var Parameters = ParameterRecordDataItem?.map((element) => {
            var Index = element.$.Index;
            var Length = element.$.Length;
            var Name = getTextById(gsdml, element.Name[0].$.TextId);
            return { Index, Length, Name };
        });

        // Must create a default parameter
        if (Parameters === undefined) {
            Parameters = [

            ]
        }

        return { ID, Name, InfoText, Parameters }
    });
    
    return {ID, ModuleIdentNumber, Name, InfoText, Submodules };
}

var modulesTemplateModel = modules.map(element => {
    return GSDMLModuleToTemplateModel(gsdml, element);
});

function getTextById(gsdml, id)
{
    var externalTextList = gsdml.ISO15745Profile.ProfileBody[0].ApplicationProcess[0].ExternalTextList[0].PrimaryLanguage[0].Text;
    var result = externalTextList.find(
        (el) => {
            return el.$.TextId == id
        }
    )
    return result.$.Value;
}

var application = Object.assign({}, template)

application.vendor_id = gsdml.ISO15745Profile.ProfileBody[0].DeviceIdentity[0].$.VendorID
application.device_id = gsdml.ISO15745Profile.ProfileBody[0].DeviceIdentity[0].$.DeviceID
application.station_name =  gsdml.ISO15745Profile.ProfileBody[0].ApplicationProcess[0].DeviceAccessPointList[0].DeviceAccessPointItem[0].$.DNS_CompatibleName;
application.order_id =  gsdml.ISO15745Profile.ProfileBody[0].ApplicationProcess[0].DeviceAccessPointList[0].DeviceAccessPointItem[0].$.OrderNumber;
application.modules = modulesTemplateModel

console.log(util.inspect(application, {showHidden: false, depth: null, colors: true}))