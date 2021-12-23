cmake_minimum_required (VERSION 3.0)
project (fnc VERSION 0.1)

# The version number.
# set (FNC_VERSION_MAJOR 0)
# set (FNC_VERSION_MINOR 1)
# ${PROJECT_BINARY_DIR} is <repo>/build"
# ${PROJECT_SOURCE_DIR} is <repo>"

set (FNC_SOURCE "${PROJECT_SOURCE_DIR}/src")
set (FNC_INCLUDE "${PROJECT_SOURCE_DIR}/src")

# Used to set the location of the INI file.
if (UNIX)
	set (FNC_INIT_PATH "~/.fnc/")
else ()
	set (FNC_INIT_PATH "${PROJECT_SOURCE_DIR}")
endif ()
set (FNC_INIT_FILE "${CMAKE_PROJECT_NAME}.ini")

# Make sure 'boost' is installed:
find_package(Boost)
# Following not necessary, immediately
#	COMPONENTS "filesystem" "property_tree")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# set (CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# set (CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")
endif ()

if (APPLE)
  set (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
endif ()

# configure a header file to pass some of the CMake settings to the source code
set (FNC_CONFIG_FILE_NAME "${CMAKE_PROJECT_NAME}Config.h")

configure_file (
	"${PROJECT_SOURCE_DIR}/${FNC_CONFIG_FILE_NAME}.in"
	"${PROJECT_BINARY_DIR}/${FNC_CONFIG_FILE_NAME}"
)

set (FNCFILE_SRCS
	${FNC_SOURCE}/fnc.cpp
	${FNC_SOURCE}/calstring.cpp
	${FNC_SOURCE}/exec.cpp
	${FNC_SOURCE}/functions.cpp
	${FNC_SOURCE}/func.cpp
	${FNC_SOURCE}/num.cpp
	${FNC_SOURCE}/numUnit.cpp
	${FNC_SOURCE}/iniParser.cpp
	${FNC_SOURCE}/baseParseFile.cpp
	${FNC_SOURCE}/interactive.cpp
)

include_directories(
	${FNC_INCLUDE}
	${PROJECT_BINARY_DIR}  # NOTE: fncConfig.h
    ${Boost_INCLUDE_DIR}
)

# add the executable
add_executable(fnc
	${FNCFILE_SRCS}
)
