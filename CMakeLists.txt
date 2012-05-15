cmake_minimum_required (VERSION 2.8)
project (quickaccess)


# Custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")


# Project header
include ("project_header")


# Main settings
set (UNIT_TESTING NO CACHE BOOL "Compile binary executable for unit testing?")


# 3rdparty
find_package (Gettext REQUIRED)

# Check that Gettext msgmerge exsits
if (NOT GETTEXT_MSGMERGE_EXECUTABLE)
    message (FATAL_ERROR "Please install msgmerge binary")
endif ()

# Check that Gettext msgfmt exsits
if (NOT GETTEXT_MSGFMT_EXECUTABLE)
    message (FATAL_ERROR "Please install msgfmt binary")
endif ()

find_package (KDE4 REQUIRED)
include (KDE4Defaults)


# Includes
include_directories (${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

# Link directories

# Libs
 
# Definitions
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


# Translations
include (MacroOptionalAddSubdirectory)
macro_optional_add_subdirectory ("po")


# Sources
add_subdirectory ("ui")
add_subdirectory ("src")
kde4_add_ui_files (quickaccess_SOURCES ${quickaccess_UIS})
kde4_add_plugin (plasma_applet_quickaccess ${quickaccess_SOURCES})
target_link_libraries (plasma_applet_quickaccess ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kfile konq)

# It is because of Qt (qglobal.h)
set_target_properties (plasma_applet_quickaccess PROPERTIES COMPILE_FLAGS "-Wno-long-long")


# Install
install (
    TARGETS plasma_applet_quickaccess
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (
    FILES "desktop/plasma-applet-quickaccess.desktop"
    DESTINATION ${SERVICES_INSTALL_DIR}
)
