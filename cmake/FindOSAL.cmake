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

# Find OSAL

find_path(OSAL_INCLUDE_DIR rtlabs/osal/osal.h)
find_library(OSAL_LIBRARY osal)
mark_as_advanced(OSAL_INCLUDE_DIR OSAL_LIBRARY)

find_package_handle_standard_args(OSAL
  REQUIRED_VARS OSAL_LIBRARY OSAL_INCLUDE_DIR
  )

if (OSAL_FOUND AND NOT TARGET OSAL::OSAL)
  add_library(OSAL::OSAL UNKNOWN IMPORTED)
  set_target_properties(OSAL::OSAL PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${OSAL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OSAL_INCLUDE_DIR}"
    )
endif()