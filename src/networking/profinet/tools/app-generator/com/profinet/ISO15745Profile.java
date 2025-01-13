package com.profinet;

import java.util.List;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ISO15745Profile {
    private ProfileHeader profileHeader;
    private ProfileBody profileBody;

    // Getters and setters
    public ProfileHeader getProfileHeader() {
        return profileHeader;
    }

    @XmlElement
    public void setProfileHeader(ProfileHeader profileHeader) {
        this.profileHeader = profileHeader;
    }

    public ProfileBody getProfileBody() {
        return profileBody;
    }

    @XmlElement
    public void setProfileBody(ProfileBody profileBody) {
        this.profileBody = profileBody;
    }

    // Other nested classes as defined earlier
    
    public static class ProfileHeader {
        private String profileIdentification;

        // Getters and setters
        public String getProfileIdentification() {
            return profileIdentification;
        }

        @XmlElement
        public void setProfileIdentification(String profileIdentification) {
            this.profileIdentification = profileIdentification;
        }
    }

    public static class ProfileBody {
        private DeviceIdentity deviceIdentity;

        // Getters and setters
        public DeviceIdentity getDeviceIdentity() {
            return deviceIdentity;
        }

        @XmlElement
        public void setDeviceIdentity(DeviceIdentity deviceIdentity) {
            this.deviceIdentity = deviceIdentity;
        }
    }

    // public static class ProfileHeader {
    //     private String profileIdentification;
    //     private String profileRevision;
    //     private String profileName;
    //     private String profileSource;
    //     private String profileClassID;
    //     private ISO15745Reference iso15745Reference;

    //     // Getters and setters
    // }

    public static class ISO15745Reference {
        private String iso15745Part;
        private String iso15745Edition;
        private String profileTechnology;

        // Getters and setters
    }

    // public static class ProfileBody {
    //     private DeviceIdentity deviceIdentity;
    //     private DeviceFunction deviceFunction;
    //     private ApplicationProcess applicationProcess;

    //     // Getters and setters
    // }

    public static class DeviceIdentity {
        private String vendorID;
        private String deviceID;
        private InfoText infoText;
        private VendorName vendorName;

        // Getters and setters
    }

    public static class InfoText {
        private String textId;

        // Getters and setters
    }

    public static class VendorName {
        private String value;

        // Getters and setters
    }

    public static class DeviceFunction {
        private String mainFamily;
        private String productFamily;

        // Getters and setters
    }

    public static class ApplicationProcess {
        private DeviceAccessPointList deviceAccessPointList;
        private ModuleList moduleList;
        private LogBookEntryList logBookEntryList;
        private GraphicsList graphicsList;
        private ExternalTextList externalTextList;

        // Getters and setters
    }

    public static class DeviceAccessPointList {
        private List<DeviceAccessPointItem> deviceAccessPointItem;

        // Getters and setters
    }

    public static class DeviceAccessPointItem {
        private String id;
        private String pnioVersion;
        private String physicalSlots;
        private String moduleIdentNumber;
        private String minDeviceInterval;
        private String dnsCompatibleName;
        private String fixedInSlots;
        private String objectUUIDLocalIndex;
        private boolean deviceAccessSupported;
        private boolean multipleWriteSupported;
        private boolean checkDeviceIDAllowed;
        private boolean nameOfStationNotTransferable;
        private boolean lldpNoDSupported;
        private String resetToFactoryModes;
        private ModuleInfo moduleInfo;
        private CertificationInfo certificationInfo;
        private IOConfigData ioConfigData;
        private UseableModules useableModules;
        private VirtualSubmoduleList virtualSubmoduleList;
        private SystemDefinedSubmoduleList systemDefinedSubmoduleList;
        private Graphics graphics;

        // Getters and setters
    }

    public static class ModuleInfo {
        private String name;
        private String infoText;
        private String vendorName;
        private String orderNumber;
        private String hardwareRelease;
        private String softwareRelease;

        // Getters and setters
    }

    public static class CertificationInfo {
        private String conformanceClass;
        private String applicationClass;
        private String netloadClass;

        // Getters and setters
    }

    public static class IOConfigData {
        private String maxInputLength;
        private String maxOutputLength;

        // Getters and setters
    }

    public static class UseableModules {
        private List<ModuleItemRef> moduleItemRef;

        // Getters and setters
    }

    public static class ModuleItemRef {
        private String moduleItemTarget;
        private String allowedInSlots;

        // Getters and setters
    }

    public static class VirtualSubmoduleList {
        private List<VirtualSubmoduleItem> virtualSubmoduleItem;

        // Getters and setters
    }

    public static class VirtualSubmoduleItem {
        private String id;
        private String submoduleIdentNumber;
        private String writeableIMRecords;
        private boolean mayIssueProcessAlarm;
        private IOData ioData;
        private ModuleInfo moduleInfo;

        // Getters and setters
    }

    public static class IOData {
        private List<DataItem> input;
        private List<DataItem> output;

        // Getters and setters
    }

    public static class DataItem {
        private String dataType;
        private String textId;

        // Getters and setters
    }

    public static class SystemDefinedSubmoduleList {
        private List<InterfaceSubmoduleItem> interfaceSubmoduleItem;
        private List<PortSubmoduleItem> portSubmoduleItem;

        // Getters and setters
    }

    public static class InterfaceSubmoduleItem {
        private String id;
        private String submoduleIdentNumber;
        private String subslotNumber;
        private String textId;
        private String supportedRTClasses;
        private String supportedProtocols;
        private boolean networkComponentDiagnosisSupported;
        private boolean ptpBoundarySupported;
        private boolean dcpBoundarySupported;
        private ApplicationRelations applicationRelations;

        // Getters and setters
    }

    public static class ApplicationRelations {
        private String startupMode;
        private TimingProperties timingProperties;

        // Getters and setters
    }

    public static class TimingProperties {
        private String sendClock;
        private String reductionRatio;

        // Getters and setters
    }

    public static class PortSubmoduleItem {
        private String id;
        private String submoduleIdentNumber;
        private String subslotNumber;
        private String textId;
        private String maxPortRxDelay;
        private String maxPortTxDelay;
        private MAUTypeList mauTypeList;

        // Getters and setters
    }

    public static class MAUTypeList {
        private List<String> mauTypeItem;

        // Getters and setters
    }

    public static class Graphics {
        private GraphicItemRef graphicItemRef;

        // Getters and setters
    }

    public static class GraphicItemRef {
        private String type;
        private String graphicItemTarget;

        // Getters and setters
    }

    public static class ModuleList {
        private List<ModuleItem> moduleItem;

        // Getters and setters
    }

    public static class ModuleItem {
        private String id;
        private String moduleIdentNumber;
        private ModuleInfo moduleInfo;
        private VirtualSubmoduleList virtualSubmoduleList;

        // Getters and setters
    }

    public static class LogBookEntryList {
        private List<LogBookEntryItem> logBookEntryItem;

        // Getters and setters
    }

    public static class LogBookEntryItem {
        private String status;
        private ErrorCode2Value errorCode2Value;

        // Getters and setters
    }

    public static class ErrorCode2Value {
        private String name;

        // Getters and setters
    }

    public static class GraphicsList {
        private List<GraphicItem> graphicItem;

        // Getters and setters
    }

    public static class GraphicItem {
        private String id;
        private String graphicFile;

        // Getters and setters
    }

    public static class ExternalTextList {
        private PrimaryLanguage primaryLanguage;

        // Getters and setters
    }

    public static class PrimaryLanguage {
        private List<Text> text;

        // Getters and setters
    }

    public static class Text {
        private String textId;
        private String value;

        // Getters and setters
    }
}