# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProgram)

# Set C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Add more source files here if needed
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/general_agent.cpp ${CMAKE_SOURCE_DIR}/src/specialised_agents.cpp ${CMAKE_SOURCE_DIR}/src/matrix.cpp ${CMAKE_SOURCE_DIR}/src/fileio.cpp)

add_executable(my_program ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/main.cpp)

####################### TESTING STUFF STARTS HERE ###########################################################

enable_testing()
add_subdirectory(tests)