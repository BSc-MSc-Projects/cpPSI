cmake_minimum_required(VERSION 3.23)
project(psi_scheme)

find_package(SEAL)
# Create a library with the necessary files
file(GLOB LIB_SOURCES src/lib/*.cpp)

# Add test executable
add_executable(test src/test/test.cpp ${LIB_SOURCES})

target_link_libraries(test SEAL::seal)

# Set the output dir for `test` binary file in bin directory
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
