# CMake tools
include ("collect_sources")


collect_sources (src_HEADERS src_SOURCES ${CMAKE_SOURCE_DIR})

# Headers
set (quickaccess_HEADERS ${quickaccess_HEADERS} ${src_HEADERS} PARENT_SCOPE)

# Sources
set (quickaccess_SOURCES ${quickaccess_SOURCES} ${src_SOURCES} PARENT_SCOPE)
