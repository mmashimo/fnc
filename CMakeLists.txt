cmake_minimum_required (VERSION 3.14)
project (fnc VERSION 0.3)

# The version number.
# set (FNC_VERSION_MAJOR 0)
# set (FNC_VERSION_MINOR 1)
# ${PROJECT_BINARY_DIR} is <repo>/build"
# ${PROJECT_SOURCE_DIR} is <repo>"

set (FNC_SOURCE "${PROJECT_SOURCE_DIR}/src")
set (FNC_INCLUDE "${PROJECT_SOURCE_DIR}/src")
set (FNC_TEST   "${PROJECT_SOURCE_DIR}/test")

# Used to set the location of the INI file.
if (UNIX)
	set (FNC_INIT_PATH "~/.fnc/")
else ()
	set (FNC_INIT_PATH "${PROJECT_SOURCE_DIR}")
endif ()
set (FNC_INIT_FILE "${CMAKE_PROJECT_NAME}.ini")

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

#FetchContent_Declare(
#    googletest
#    GIT_REPOSITORY https://github.com/google/googletest.git
#    GIT_TAG        release-1.8.0
#)

#FetchContent_GetProperties(googletest)
#  if(NOT googletest_POPULATED)
#    FetchContent_Populate(googletest)
#    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
#  endif()

#FetchContent_Populate( googletest )


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

# Functions core to fnc
set (FNCFILE_SRCS
	${FNC_SOURCE}/xstring.cpp
	${FNC_SOURCE}/calstring.cpp
	${FNC_SOURCE}/calclist.cpp
	${FNC_SOURCE}/exec.cpp
	${FNC_SOURCE}/functions.cpp
	${FNC_SOURCE}/func.cpp
	${FNC_SOURCE}/num.cpp
	${FNC_SOURCE}/numUnit.cpp
)

# Functions used for main
set (FNCMAIN_SRCS
	${FNC_SOURCE}/fnc.cpp
	${FNC_SOURCE}/iniParser.cpp
	${FNC_SOURCE}/baseParseFile.cpp
	${FNC_SOURCE}/interactive.cpp
	${FNC_SOURCE}/execInteractive.cpp
)

include_directories(
	${FNC_INCLUDE}
	${PROJECT_BINARY_DIR}  # NOTE: fncConfig.h
    ${Boost_INCLUDE_DIR}
)

# add the executable
add_executable(fnc
	${FNCMAIN_SRCS}
	${FNCFILE_SRCS}
)


# Add Testing
include(${FNC_TEST}/CMakeLists.txt)

