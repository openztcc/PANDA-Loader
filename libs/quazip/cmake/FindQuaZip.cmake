find_path(QuaZip_INCLUDE_DIR quazip.h
    PATHS ${CMAKE_SOURCE_DIR}/libs/quazip/include
    PATH_SUFFIXES quazip
)

find_library(QuaZip_LIBRARY NAMES quazip
    PATHS ${CMAKE_SOURCE_DIR}/libs/quazip/lib
)

if(QuaZip_INCLUDE_DIR AND QuaZip_LIBRARY)
    set(QuaZip_FOUND TRUE)
else()
    set(QuaZip_FOUND FALSE)
endif()

mark_as_advanced(QuaZip_INCLUDE_DIR QuaZip_LIBRARY)

if(QuaZip_FOUND)
    add_library(QuaZip::QuaZip UNKNOWN IMPORTED)
    set_target_properties(QuaZip::QuaZip PROPERTIES
        IMPORTED_LOCATION "${QuaZip_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${QuaZip_INCLUDE_DIR}"
    )
endif()
