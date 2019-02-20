cmake_minimum_required(VERSION 3.0)

project (Mini-STL)

# set version number 
SET(VERESION_MAJOR 0)
SET(VERSION_MID 4)
SET(VERSION_MINOR 3)

# set paths
SET(Template_dir ${CMAKE_SOURCE_DIR}/Templates)
SET(Header_dir ${CMAKE_SOURCE_DIR}/Header)
SET(Source_dir ${CMAKE_SOURCE_DIR}/Src)

# Cpp standard setting
SET(CPP_STD 11)


# load config header Templates
configure_file(
	${Template_dir}/Version.h.in 
	${Header_dir}/Config/Version.h
)

configure_file(
	${Template_dir}/Config.h.in
	${Header_dir}/Config/Config.h
)

# Complie options
option(USE_DIRECT_MALLOC
		"Whether to use simple malloc strategy" OFF)

option(USE_EXCEPTION
		"Whether to use exception handling" ON)

# add include
include_directories(${Header_dir})

add_subdirectory(${Source_dir})