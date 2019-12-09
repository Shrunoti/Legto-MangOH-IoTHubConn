#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jansson" for configuration ""
set_property(TARGET jansson APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(jansson PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/lib/libjansson.so"
  IMPORTED_SONAME_NOCONFIG "libjansson.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS jansson )
list(APPEND _IMPORT_CHECK_FILES_FOR_jansson "/usr/local/lib/libjansson.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
