var data = {
    "ISO15745Profile": {
        "$": {
            "xmlns": "http://www.profibus.com/GSDML/2003/11/DeviceProfile",
            "xmlns:xsi": "http://www.w3.org/2001/XMLSchema-instance",
            "xsi:schemaLocation": "http://www.profibus.com/GSDML/2003/11/DeviceProfile ..\\xsd\\GSDML-DeviceProfile-V2.4.xsd"
        },
        "ProfileHeader": [
            {
                "ProfileIdentification": [
                    "PROFINET Device Profile"
                ],
                "ProfileRevision": [
                    "1.00"
                ],
                "ProfileName": [
                    "Device Profile for PROFINET Devices"
                ],
                "ProfileSource": [
                    "PROFIBUS Nutzerorganisation e. V. (PNO)"
                ],
                "ProfileClassID": [
                    "Device"
                ],
                "ISO15745Reference": [
                    {
                        "ISO15745Part": [
                            "4"
                        ],
                        "ISO15745Edition": [
                            "1"
                        ],
                        "ProfileTechnology": [
                            "GSDML"
                        ]
                    }
                ]
            }
        ],
        "ProfileBody": [
            {
                "DeviceIdentity": [
                    {
                        "$": {
                            "VendorID": "0x0493",
                            "DeviceID": "0x0002"
                        },
                        "InfoText": [
                            {
                                "$": {
                                    "TextId": "IDT_INFO_Device"
                                }
                            }
                        ],
                        "VendorName": [
                            {
                                "$": {
                                    "Value": "RT-Labs"
                                }
                            }
                        ]
                    }
                ],
                "DeviceFunction": [
                    {
                        "Family": [
                            {
                                "$": {
                                    "MainFamily": "I/O",
                                    "ProductFamily": "P-Net Samples"
                                }
                            }
                        ]
                    }
                ],
                "ApplicationProcess": [
                    {
                        "DeviceAccessPointList": [
                            {
                                "DeviceAccessPointItem": [
                                    {
                                        "$": {
                                            "ID": "IDD_1",
                                            "PNIO_Version": "V2.4",
                                            "PhysicalSlots": "0..4",
                                            "ModuleIdentNumber": "0x00000001",
                                            "MinDeviceInterval": "32",
                                            "DNS_CompatibleName": "rt-labs-dev",
                                            "FixedInSlots": "0",
                                            "ObjectUUID_LocalIndex": "1",
                                            "DeviceAccessSupported": "false",
                                            "MultipleWriteSupported": "true",
                                            "CheckDeviceID_Allowed": "true",
                                            "NameOfStationNotTransferable": "false",
                                            "LLDP_NoD_Supported": "true",
                                            "ResetToFactoryModes": "1..2"
                                        },
                                        "ModuleInfo": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "IDT_MODULE_NAME_DAP1"
                                                        }
                                                    }
                                                ],
                                                "InfoText": [
                                                    {
                                                        "$": {
                                                            "TextId": "IDT_INFO_DAP1"
                                                        }
                                                    }
                                                ],
                                                "VendorName": [
                                                    {
                                                        "$": {
                                                            "Value": "RT-Labs"
                                                        }
                                                    }
                                                ],
                                                "OrderNumber": [
                                                    {
                                                        "$": {
                                                            "Value": "12345 Abcdefghijk"
                                                        }
                                                    }
                                                ],
                                                "HardwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "A1.0"
                                                        }
                                                    }
                                                ],
                                                "SoftwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "V0.1.0"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "CertificationInfo": [
                                            {
                                                "$": {
                                                    "ConformanceClass": "B",
                                                    "ApplicationClass": "",
                                                    "NetloadClass": "I"
                                                }
                                            }
                                        ],
                                        "IOConfigData": [
                                            {
                                                "$": {
                                                    "MaxInputLength": "244",
                                                    "MaxOutputLength": "244"
                                                }
                                            }
                                        ],
                                        "UseableModules": [
                                            {
                                                "ModuleItemRef": [
                                                    {
                                                        "$": {
                                                            "ModuleItemTarget": "IDM_30",
                                                            "AllowedInSlots": "1..4"
                                                        }
                                                    },
                                                    {
                                                        "$": {
                                                            "ModuleItemTarget": "IDM_31",
                                                            "AllowedInSlots": "1..4"
                                                        }
                                                    },
                                                    {
                                                        "$": {
                                                            "ModuleItemTarget": "IDM_32",
                                                            "AllowedInSlots": "1..4"
                                                        }
                                                    },
                                                    {
                                                        "$": {
                                                            "ModuleItemTarget": "IDM_40",
                                                            "AllowedInSlots": "1..4"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "VirtualSubmoduleList": [
                                            {
                                                "VirtualSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDS_1",
                                                            "SubmoduleIdentNumber": "0x00000001",
                                                            "Writeable_IM_Records": "1 2 3",
                                                            "MayIssueProcessAlarm": "false"
                                                        },
                                                        "IOData": [
                                                            ""
                                                        ],
                                                        "ModuleInfo": [
                                                            {
                                                                "Name": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "IDT_MODULE_NAME_DAP1"
                                                                        }
                                                                    }
                                                                ],
                                                                "InfoText": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "IDT_INFO_DAP1"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ],
                                        "SystemDefinedSubmoduleList": [
                                            {
                                                "InterfaceSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDS_I",
                                                            "SubmoduleIdentNumber": "0x00008000",
                                                            "SubslotNumber": "32768",
                                                            "TextId": "IDT_NAME_IS",
                                                            "SupportedRT_Classes": "RT_CLASS_1",
                                                            "SupportedProtocols": "SNMP;LLDP",
                                                            "NetworkComponentDiagnosisSupported": "false",
                                                            "PTP_BoundarySupported": "true",
                                                            "DCP_BoundarySupported": "true"
                                                        },
                                                        "ApplicationRelations": [
                                                            {
                                                                "$": {
                                                                    "StartupMode": "Advanced"
                                                                },
                                                                "TimingProperties": [
                                                                    {
                                                                        "$": {
                                                                            "SendClock": "32",
                                                                            "ReductionRatio": "1 2 4 8 16 32 64 128 256 512"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ],
                                                "PortSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDS_P1",
                                                            "SubmoduleIdentNumber": "0x00008001",
                                                            "SubslotNumber": "32769",
                                                            "TextId": "IDT_NAME_PS1",
                                                            "MaxPortRxDelay": "350",
                                                            "MaxPortTxDelay": "160"
                                                        },
                                                        "MAUTypeList": [
                                                            {
                                                                "MAUTypeItem": [
                                                                    {
                                                                        "$": {
                                                                            "Value": "30"
                                                                        }
                                                                    },
                                                                    {
                                                                        "$": {
                                                                            "Value": "16"
                                                                        }
                                                                    },
                                                                    {
                                                                        "$": {
                                                                            "Value": "5"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ],
                                        "Graphics": [
                                            {
                                                "GraphicItemRef": [
                                                    {
                                                        "$": {
                                                            "Type": "DeviceSymbol",
                                                            "GraphicItemTarget": "RT-LabsStackImage"
                                                        }
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            }
                        ],
                        "ModuleList": [
                            {
                                "ModuleItem": [
                                    {
                                        "$": {
                                            "ID": "IDM_30",
                                            "ModuleIdentNumber": "0x00000030"
                                        },
                                        "ModuleInfo": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_Name_Module_I8"
                                                        }
                                                    }
                                                ],
                                                "InfoText": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_InfoText_Module_I8"
                                                        }
                                                    }
                                                ],
                                                "HardwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ],
                                                "SoftwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "VirtualSubmoduleList": [
                                            {
                                                "VirtualSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDSM_130",
                                                            "SubmoduleIdentNumber": "0x0130",
                                                            "MayIssueProcessAlarm": "true"
                                                        },
                                                        "IOData": [
                                                            {
                                                                "Input": [
                                                                    {
                                                                        "$": {
                                                                            "Consistency": "All items consistency"
                                                                        },
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned8",
                                                                                    "TextId": "TOK_Input_DataItem_8",
                                                                                    "UseAsBits": "true"
                                                                                },
                                                                                "BitDataItem": [
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "0",
                                                                                            "TextId": "TOK_Input_DataItem_Bit0"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "1",
                                                                                            "TextId": "TOK_Input_DataItem_Bit1"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "2",
                                                                                            "TextId": "TOK_Input_DataItem_Bit2"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "3",
                                                                                            "TextId": "TOK_Input_DataItem_Bit3"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "4",
                                                                                            "TextId": "TOK_Input_DataItem_Bit4"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "5",
                                                                                            "TextId": "TOK_Input_DataItem_Bit5"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "6",
                                                                                            "TextId": "TOK_Input_DataItem_Bit6"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "7",
                                                                                            "TextId": "TOK_Input_DataItem_Bit7"
                                                                                        }
                                                                                    }
                                                                                ]
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "ModuleInfo": [
                                                            {
                                                                "Name": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_Name_Module_I8"
                                                                        }
                                                                    }
                                                                ],
                                                                "InfoText": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_InfoText_Module_I8"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    },
                                    {
                                        "$": {
                                            "ID": "IDM_31",
                                            "ModuleIdentNumber": "0x00000031"
                                        },
                                        "ModuleInfo": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_Name_Module_O8"
                                                        }
                                                    }
                                                ],
                                                "InfoText": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_InfoText_Module_O8"
                                                        }
                                                    }
                                                ],
                                                "HardwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ],
                                                "SoftwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "VirtualSubmoduleList": [
                                            {
                                                "VirtualSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDSM_131",
                                                            "SubmoduleIdentNumber": "0x0131",
                                                            "MayIssueProcessAlarm": "true"
                                                        },
                                                        "IOData": [
                                                            {
                                                                "Output": [
                                                                    {
                                                                        "$": {
                                                                            "Consistency": "All items consistency"
                                                                        },
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned8",
                                                                                    "TextId": "TOK_Output_DataItem_8",
                                                                                    "UseAsBits": "true"
                                                                                },
                                                                                "BitDataItem": [
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "0",
                                                                                            "TextId": "TOK_Output_DataItem_Bit0"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "1",
                                                                                            "TextId": "TOK_Output_DataItem_Bit1"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "2",
                                                                                            "TextId": "TOK_Output_DataItem_Bit2"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "3",
                                                                                            "TextId": "TOK_Output_DataItem_Bit3"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "4",
                                                                                            "TextId": "TOK_Output_DataItem_Bit4"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "5",
                                                                                            "TextId": "TOK_Output_DataItem_Bit5"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "6",
                                                                                            "TextId": "TOK_Output_DataItem_Bit6"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "7",
                                                                                            "TextId": "TOK_Output_DataItem_Bit7"
                                                                                        }
                                                                                    }
                                                                                ]
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "ModuleInfo": [
                                                            {
                                                                "Name": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_Name_Module_O8"
                                                                        }
                                                                    }
                                                                ],
                                                                "InfoText": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_InfoText_Module_O8"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    },
                                    {
                                        "$": {
                                            "ID": "IDM_32",
                                            "ModuleIdentNumber": "0x00000032"
                                        },
                                        "ModuleInfo": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_Name_Module_I8O8"
                                                        }
                                                    }
                                                ],
                                                "InfoText": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_InfoText_Module_I8O8"
                                                        }
                                                    }
                                                ],
                                                "HardwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ],
                                                "SoftwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "VirtualSubmoduleList": [
                                            {
                                                "VirtualSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDSM_132",
                                                            "SubmoduleIdentNumber": "0x0132",
                                                            "MayIssueProcessAlarm": "true"
                                                        },
                                                        "IOData": [
                                                            {
                                                                "Input": [
                                                                    {
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned8",
                                                                                    "UseAsBits": "true",
                                                                                    "TextId": "TOK_Input_DataItem_8"
                                                                                },
                                                                                "BitDataItem": [
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "0",
                                                                                            "TextId": "TOK_Input_DataItem_Bit0"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "1",
                                                                                            "TextId": "TOK_Input_DataItem_Bit1"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "2",
                                                                                            "TextId": "TOK_Input_DataItem_Bit2"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "3",
                                                                                            "TextId": "TOK_Input_DataItem_Bit3"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "4",
                                                                                            "TextId": "TOK_Input_DataItem_Bit4"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "5",
                                                                                            "TextId": "TOK_Input_DataItem_Bit5"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "6",
                                                                                            "TextId": "TOK_Input_DataItem_Bit6"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "7",
                                                                                            "TextId": "TOK_Input_DataItem_Bit7"
                                                                                        }
                                                                                    }
                                                                                ]
                                                                            }
                                                                        ]
                                                                    }
                                                                ],
                                                                "Output": [
                                                                    {
                                                                        "$": {
                                                                            "Consistency": "All items consistency"
                                                                        },
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned8",
                                                                                    "TextId": "TOK_Output_DataItem_8",
                                                                                    "UseAsBits": "true"
                                                                                },
                                                                                "BitDataItem": [
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "0",
                                                                                            "TextId": "TOK_Output_DataItem_Bit0"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "1",
                                                                                            "TextId": "TOK_Output_DataItem_Bit1"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "2",
                                                                                            "TextId": "TOK_Output_DataItem_Bit2"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "3",
                                                                                            "TextId": "TOK_Output_DataItem_Bit3"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "4",
                                                                                            "TextId": "TOK_Output_DataItem_Bit4"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "5",
                                                                                            "TextId": "TOK_Output_DataItem_Bit5"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "6",
                                                                                            "TextId": "TOK_Output_DataItem_Bit6"
                                                                                        }
                                                                                    },
                                                                                    {
                                                                                        "$": {
                                                                                            "BitOffset": "7",
                                                                                            "TextId": "TOK_Output_DataItem_Bit7"
                                                                                        }
                                                                                    }
                                                                                ]
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "RecordDataList": [
                                                            {
                                                                "ParameterRecordDataItem": [
                                                                    {
                                                                        "$": {
                                                                            "Index": "123",
                                                                            "Length": "4"
                                                                        },
                                                                        "Name": [
                                                                            {
                                                                                "$": {
                                                                                    "TextId": "TOK_sample_parameter_1"
                                                                                }
                                                                            }
                                                                        ],
                                                                        "Ref": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned32",
                                                                                    "ByteOffset": "0",
                                                                                    "DefaultValue": "1",
                                                                                    "AllowedValues": "0..99",
                                                                                    "Changeable": "true",
                                                                                    "Visible": "true",
                                                                                    "TextId": "TOK_Demo_1"
                                                                                }
                                                                            }
                                                                        ]
                                                                    },
                                                                    {
                                                                        "$": {
                                                                            "Index": "124",
                                                                            "Length": "4"
                                                                        },
                                                                        "Name": [
                                                                            {
                                                                                "$": {
                                                                                    "TextId": "TOK_sample_parameter_2"
                                                                                }
                                                                            }
                                                                        ],
                                                                        "Ref": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned32",
                                                                                    "ByteOffset": "0",
                                                                                    "DefaultValue": "2",
                                                                                    "AllowedValues": "0..999",
                                                                                    "Changeable": "true",
                                                                                    "Visible": "true",
                                                                                    "TextId": "TOK_Demo_2"
                                                                                }
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "ModuleInfo": [
                                                            {
                                                                "Name": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_Name_Module_I8O8"
                                                                        }
                                                                    }
                                                                ],
                                                                "InfoText": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_InfoText_Module_I8O8"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    },
                                    {
                                        "$": {
                                            "ID": "IDM_40",
                                            "ModuleIdentNumber": "0x00000040"
                                        },
                                        "ModuleInfo": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_Name_Module_Echo"
                                                        }
                                                    }
                                                ],
                                                "InfoText": [
                                                    {
                                                        "$": {
                                                            "TextId": "TOK_InfoText_Module_Echo"
                                                        }
                                                    }
                                                ],
                                                "HardwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ],
                                                "SoftwareRelease": [
                                                    {
                                                        "$": {
                                                            "Value": "1.0"
                                                        }
                                                    }
                                                ]
                                            }
                                        ],
                                        "VirtualSubmoduleList": [
                                            {
                                                "VirtualSubmoduleItem": [
                                                    {
                                                        "$": {
                                                            "ID": "IDSM_140",
                                                            "SubmoduleIdentNumber": "0x0140",
                                                            "MayIssueProcessAlarm": "true"
                                                        },
                                                        "IOData": [
                                                            {
                                                                "Input": [
                                                                    {
                                                                        "$": {
                                                                            "Consistency": "All items consistency"
                                                                        },
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Float32",
                                                                                    "TextId": "TOK_Input_DataItem_Echo_Float32"
                                                                                }
                                                                            },
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned32",
                                                                                    "TextId": "TOK_Input_DataItem_Echo_Unsigned32"
                                                                                }
                                                                            }
                                                                        ]
                                                                    }
                                                                ],
                                                                "Output": [
                                                                    {
                                                                        "$": {
                                                                            "Consistency": "All items consistency"
                                                                        },
                                                                        "DataItem": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Float32",
                                                                                    "TextId": "TOK_Output_DataItem_Echo_Float32"
                                                                                }
                                                                            },
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned32",
                                                                                    "TextId": "TOK_Output_DataItem_Echo_Unsigned32"
                                                                                }
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "RecordDataList": [
                                                            {
                                                                "ParameterRecordDataItem": [
                                                                    {
                                                                        "$": {
                                                                            "Index": "125",
                                                                            "Length": "4"
                                                                        },
                                                                        "Name": [
                                                                            {
                                                                                "$": {
                                                                                    "TextId": "TOK_sample_parameter_Echo"
                                                                                }
                                                                            }
                                                                        ],
                                                                        "Ref": [
                                                                            {
                                                                                "$": {
                                                                                    "DataType": "Unsigned32",
                                                                                    "ByteOffset": "0",
                                                                                    "DefaultValue": "2",
                                                                                    "AllowedValues": "1..4",
                                                                                    "Changeable": "true",
                                                                                    "Visible": "true",
                                                                                    "TextId": "TOK_Echo_Gain"
                                                                                }
                                                                            }
                                                                        ]
                                                                    }
                                                                ]
                                                            }
                                                        ],
                                                        "ModuleInfo": [
                                                            {
                                                                "Name": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_Name_Module_Echo"
                                                                        }
                                                                    }
                                                                ],
                                                                "InfoText": [
                                                                    {
                                                                        "$": {
                                                                            "TextId": "TOK_InfoText_Module_Echo"
                                                                        }
                                                                    }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            }
                        ],
                        "LogBookEntryList": [
                            {
                                "LogBookEntryItem": [
                                    {
                                        "$": {
                                            "Status": "2130510"
                                        },
                                        "ErrorCode2Value": [
                                            {
                                                "Name": [
                                                    {
                                                        "$": {
                                                            "TextId": "IDT_CUSTOM_LOGBOOK_1"
                                                        }
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            }
                        ],
                        "GraphicsList": [
                            {
                                "GraphicItem": [
                                    {
                                        "$": {
                                            "ID": "RT-LabsStackImage",
                                            "GraphicFile": "GSDML-RT-LABS-STACK"
                                        }
                                    }
                                ]
                            }
                        ],
                        "ExternalTextList": [
                            {
                                "PrimaryLanguage": [
                                    {
                                        "Text": [
                                            {
                                                "$": {
                                                    "TextId": "IDT_INFO_Device",
                                                    "Value": "https://github.com/rtlabs-com/p-net"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_MODULE_NAME_DAP1",
                                                    "Value": "P-Net multi-module sample app"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_INFO_DAP1",
                                                    "Value": "Profinet device sample app https://github.com/rtlabs-com/p-net"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_CUSTOM_DIAG_1",
                                                    "Value": "Custom diagnosis in USI format"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_CUSTOM_DIAG_1_VALUE",
                                                    "Value": "Custom diagnosis value"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_CUSTOM_LOGBOOK_1",
                                                    "Value": "Custom Logbook entry"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_NAME_IS",
                                                    "Value": "X1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_NAME_PS1",
                                                    "Value": "X1 P1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "IDT_NAME_PS2",
                                                    "Value": "X1 P2"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Name_Module_I8",
                                                    "Value": "DI 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Name_Module_O8",
                                                    "Value": "DO 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Name_Module_I8O8",
                                                    "Value": "DIO 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Name_Module_Echo",
                                                    "Value": "Echo Module"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_InfoText_Module_I8",
                                                    "Value": "Digital In 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_InfoText_Module_O8",
                                                    "Value": "Digital Out 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_InfoText_Module_I8O8",
                                                    "Value": "Digital In+Out 8xLogicLevel"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_InfoText_Module_Echo",
                                                    "Value": "Echo with adjustable gain"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_8",
                                                    "Value": "Input 8 bits"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_8",
                                                    "Value": "Output 8 bits"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit0",
                                                    "Value": "Input Bit 0"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit1",
                                                    "Value": "Input Bit 1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit2",
                                                    "Value": "Input Bit 2"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit3",
                                                    "Value": "Input Bit 3"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit4",
                                                    "Value": "Input Bit 4"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit5",
                                                    "Value": "Input Bit 5"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit6",
                                                    "Value": "Input Bit 6"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Bit7",
                                                    "Value": "Input Bit 7"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit0",
                                                    "Value": "Output Bit 0"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit1",
                                                    "Value": "Output Bit 1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit2",
                                                    "Value": "Output Bit 2"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit3",
                                                    "Value": "Output Bit 3"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit4",
                                                    "Value": "Output Bit 4"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit5",
                                                    "Value": "Output Bit 5"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit6",
                                                    "Value": "Output Bit 6"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Bit7",
                                                    "Value": "Output Bit 7"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Echo_Float32",
                                                    "Value": "Input float to controller (output from controller multiplied by gain)"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Input_DataItem_Echo_Unsigned32",
                                                    "Value": "Input int to controller (output from controller multiplied by gain)"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Echo_Float32",
                                                    "Value": "Output float from controller"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Output_DataItem_Echo_Unsigned32",
                                                    "Value": "Output int from controller"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_sample_parameter_1",
                                                    "Value": "Parameter 1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_sample_parameter_2",
                                                    "Value": "Parameter 2"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_sample_parameter_Echo",
                                                    "Value": "Gain for echo module"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Demo_1",
                                                    "Value": "Demo 1"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Demo_2",
                                                    "Value": "Demo 2"
                                                }
                                            },
                                            {
                                                "$": {
                                                    "TextId": "TOK_Echo_Gain",
                                                    "Value": "Gain"
                                                }
                                            }
                                        ]
                                    }
                                ]
                            }
                        ]
                    }
                ]
            }
        ]
    }
}

module.exports = {
    data
}