# Find the libamz libs and includes.
#
# Once run this will define
#     LIBAMZ_INCLUDE_DIR - The include dir where the headrrs could be find.
#     LIBAMZ_LIBRARIES - The path to the library.
#
# Copyright (c) 2012 Christian Krippendorf <Coding@Christian-Krippendorf.de>
# Redistributed under the terms of the GPLv3

set(LIBAMZ_LIBRARIES)
set(LIBAMZ_INCLUDE_DIR)

find_path(LIBAMZ_INCLUDE_DIR amz.h PATH_SUFFIXES include)
find_library(LIBAMZ_LIBRARIES NAMES amz)

if (LIBAMZ_INCLUDE_DIR)
    message(STATUS "Found libamz headers: ${LIBAMZ_INCLUDE_DIR}")
else (LIBAMZ_INCLUDE_DIR)
    if (LibAmz_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libamz headers")
    endif (LibAmz_FIND_REQUIRED)
endif (LIBAMZ_INCLUDE_DIR)

if (LIBAMZ_LIBRARIES)
    message(STATUS "Found libamz: ${LIBAMZ_LIBRARIES}")
else (LIBAMZ_LIBRARIES)
    if (LibAmz_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libamz libraries")
    endif (LibAmz_FIND_REQUIRED)
endif (LIBAMZ_LIBRARIES)

mark_as_advanced(${LIBAMZ_INCLUDE_DIR} ${LIBAMZ_LIBRARIES})
