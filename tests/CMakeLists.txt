cmake_minimum_required(VERSION 2.6)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

project(examples C CXX ASM)

if(NOT HFRAMEWORK_PATH)
	set(HFRAMEWORK_PATH "${CURRENT_DIR}/../")
endif()

include(${HFRAMEWORK_PATH}/hFramework.cmake)

set(DEVEL 1)
if(DEVEL)
	add_component_src("${CURRENT_DIR}/../")
else()
	add_component_lib(modules "${CURRENT_DIR}/../")
endif()

file(GLOB tmp "${CURRENT_DIR}/../*/examples/*.cpp")

foreach(path ${tmp})
	get_filename_component(path ${path} REALPATH)
	string(REPLACE "/" ";" components ${path})
	string(REPLACE "." ";" components "${components}")
	list(REVERSE components)
	list(GET components 1 p1)
	list(GET components 3 p2)
	add_hexecutable("${p2}_${p1}" ${path})
endforeach(path)
