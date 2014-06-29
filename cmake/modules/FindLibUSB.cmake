# - Try to find the freetype library
# Once done this defines
#
#  LIBUSB_FOUND - system has libusb
#  LIBUSB_INCLUDES - the libusb include directory
#  LIBUSB_LIBRARIES - Link these to use libusb

# Copyright (c) 2006, 2008  Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (NOT WIN32)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  find_package(PkgConfig)
  pkg_check_modules(PC_LIBUSB QUIET libusb-1.0)
endif(NOT WIN32)

find_path(LIBUSB_INCLUDES libusb.h
  HINTS ${PC_LIBUSB_INCLUDEDIR} ${PC_LIBUSB_INCLUDE_DIRS})

find_library(LIBUSB_LIBRARIES NAMES usb-1.0
  HINTS ${PC_LIBUSB_LIBDIR} ${PC_LIBUSB_LIBRARY_DIRS})

if (PC_LIBUSB_VERSION)
    set(LIBUSB_VERSION ${PC_LIBUSB_VERSION})
endif (PC_LIBUSB_VERSION)
if (LibUSB_FIND_VERSION)
    if (${LIBUSB_VERSION} VERSION_LESS ${LibUSB_FIND_VERSION})
        if (LibUSB_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find libusb: Found unsuitable version \"${LIBUSB_VERSION}\", but required is at least \"${LibUSB_FIND_VERSION}\"")
        elseif (NOT LibUSB_FIND_QUIET)
            message(STATUS "Could NOT find libusb: Found unsuitable version \"${LIBUSB_VERSION}\", but required is at least \"${LibUSB_FIND_VERSION}\"")
        endif (LibUSB_FIND_REQUIRED)
        unset(LIBUSB_VERSION)
    elseif (LibUSB_FIND_VERSION_EXACT AND NOT ${LIBUSB_VERSION} VERSION_EQUAL ${LibUSB_FIND_VERSION})
        if (LibUSB_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find libusb: Found unsuitable version \"${LIBUSB_VERSION}\", but required is exact \"${LibUSB_FIND_VERSION}\"")
        elseif (NOT LibUSB_FIND_QUIET)
            message(STATUS "Could NOT find libusb: Found unsuitable version \"${LIBUSB_VERSION}\", but required is exact \"${LibUSB_FIND_VERSION}\"")
        endif (LibUSB_FIND_REQUIRED)
        unset(LIBUSB_VERSION)
    endif (${LIBUSB_VERSION} VERSION_LESS ${LibUSB_FIND_VERSION})
endif (LibUSB_FIND_VERSION)

if (LIBUSB_INCLUDES AND LIBUSB_LIBRARIES AND LIBUSB_VERSION)
    # libusb found
    set(LIBUSB_FOUND TRUE CACHE BOOL "Do we have libusb?" FORCE)
        # Message
    if (NOT LibUSB_FIND_QUIET)
        if (NOT LibUSB_FIND_VERSION)
            message(STATUS "Found libusb: ${LIBUSB_LIBRARIES} (found version \"${LIBUSB_VERSION}\")")
        elseif (NOT ICE_FIND_VERSION_EXACT)
            message(STATUS "Found libusb: ${LIBUSB_LIBRARIES} (found version \"${LIBUSB_VERSION}\", required is \"${LibUSB_FIND_VERSION}\")")
        else (NOT ICE_FIND_VERSION_EXACT)
            message(STATUS "Found libusb: ${LIBUSB_LIBRARIES} (found suitable exact version \"${LIBUSB_VERSION}\")")
        endif (NOT LibUSB_FIND_VERSION)
    endif (NOT LibUSB_FIND_QUIET)
else (LIBUSB_INCLUDES AND LIBUSB_LIBRARIES AND LIBUSB_VERSION)
    # libusb NOT found
    set(LIBUSB_FOUND FALSE)
    # Message
    if (LibUSB_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find libusb")
    else (ICE_FIND_REQUIRED)
        if (NOT LibUSB_FIND_QUIET)
            message(WARNING "Could NOT find libusb")
        endif (NOT LibUSB_FIND_QUIET)
    endif (LibUSB_FIND_REQUIRED)
    # Clean all
    unset(LIBUSB_INCLUDES)
    unset(LIBUSB_LIBRARIES)
    unset(LIBUSB_VERSION)
endif (LIBUSB_INCLUDES AND LIBUSB_LIBRARIES AND LIBUSB_VERSION)

mark_as_advanced(LIBUSB_INCLUDES LIBUSB_LIBRARIES LIBUSB_VERSION)
