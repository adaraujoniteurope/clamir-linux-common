var data = {
    "station_name": "rt-labs-dev",
    "vendor_id": 0x0493,
    "device_id": 0x0002,
    "oem_vendor_id": 0xCAFE,
    "oem_device_id": 0xEE02,
    "im_hardware_revision": 0x0003,
    "im_version_major": 1,
    "im_version_minor": 2,
    "sw_rev_prefix": "V",
    "profile_id": 0x1234,
    "profile_spec_type": 0x5678,
    "im_revision_counter": 0,
    "serial_number": "007",
    "tag_function": "my function",
    "tag_location": "my location",
    "im_date": "2022-03-01 10:03",
    "descriptor": "my descriptor",
    "signature": "",
    "order_id": "12345 Abcdefghijk",
    "product_name": "P-Net Sample Application",
    "min_device_interval": 32,
    "diag_custom_usi": 0x1234,
    "logbook_error_code": 32,
    "logbook_error_decode": 130,
    "logbook_error_code_2": 0,
    "logbook_entry_detail": 0xFEE1DEAD,
    "modules": [
        {
            "module": "echo",
            "description": "Echo Module",
            "idx": 125,
            "id": 2,
            "input_size": 8,
            "output_size": 8,
            "submodules": [
                {
                    "submodule": "echo",
                    "description": "Echo Submodule",
                    "direction": "PNET_DIR_IO",
                    "parameters": [
                        {
                            "parameter": "echo",
                            "description": "Echo Parameter"
                        }
                    ]
                }
            ]
        }
    ]
}

module.exports = {
    data
}