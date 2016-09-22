cmake_minimum_required(VERSION 2.6)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

project(robocore-modules C CXX ASM)

if(NOT HFRAMEWORK_PATH)
	set(HFRAMEWORK_PATH ${CURRENT_DIR}/hFramework/)
endif()

set(debug_flags "-DMODULES_DEBUG")

include(${HFRAMEWORK_PATH}/robocore_common.cmake)
include(${CURRENT_DIR}/modules.cmake)

optimization("2")

if(NOT RELEASE)
	set(TARGET_NAME modulesd)
else()
	set(TARGET_NAME modules)
endif()

add_library(${TARGET_NAME} bindir ${MODULES_SOURCES})

set(BINDIR "${CURRENT_DIR}/${LIB_DIR_NAME}")
add_custom_command(OUTPUT bindir COMMAND mkdir -p ${BINDIR})
set_target_properties(${TARGET_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${BINDIR})
