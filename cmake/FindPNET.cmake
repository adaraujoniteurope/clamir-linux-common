#********************************************************************
#        _       _         _
#  _ __ | |_  _ | |  __ _ | |__   ___
# | '__|| __|(_)| | / _` || '_ \ / __|
# | |   | |_  _ | || (_| || |_) |\__ \
# |_|    \__|(_)|_| \__,_||_.__/ |___/
#
# www.rt-labs.com
# Copyright 2020 rt-labs AB, Sweden.
#
# This software is dual-licensed under GPLv3 and a commercial
# license. See the file LICENSE.md distributed with this software for
# full license information.
#*******************************************************************/

include(FindPackageHandleStandardArgs)

# Find PNET

find_path(PNET_INCLUDE_DIR rtlabs/p-net/pnet_api.h)
find_library(PNET_LIBRARY profinet)
mark_as_advanced(PNET_INCLUDE_DIR PNET_LIBRARY)

find_package_handle_standard_args(PNET
  REQUIRED_VARS PNET_LIBRARY PNET_INCLUDE_DIR
  )

if (PNET_FOUND AND NOT TARGET PNET::PNET)
  add_library(PNET::PNET UNKNOWN IMPORTED)
  set_target_properties(PNET::PNET PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${PNET_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OSAL_INCLUDE_DIR}"
    )
endif()