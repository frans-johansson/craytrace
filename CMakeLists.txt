cmake_minimum_required(VERSION 3.10)

# Project name
project(CRaytraycer)

# C++ Standard and compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -pedantic -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNOTHREAD")

# Add source files
file(GLOB_RECURSE SRC_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES})

# Handle glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARY})

# Handle libpng
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})

# Handle png++ and other .cmake imports
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
