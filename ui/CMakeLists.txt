# CMake tools
include ("collect_files")


# UIs
collect_files (src_UIS "*.ui" ${CMAKE_SOURCE_DIR})
set (quickaccess_UIS ${quickaccess_UIS} ${src_UIS} PARENT_SCOPE)
