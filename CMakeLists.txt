cmake_minimum_required(VERSION 3.23)
project(prova_seal VERSION 1.0)
add_executable(app src/main/main.cpp src/sender/sender.cpp src/receiver/receiver.cpp src/utils/utils.cpp)
add_executable(test src/test/test.cpp src/sender/sender.cpp src/receiver/receiver.cpp src/utils/utils.cpp)

find_package(SEAL)

target_link_libraries(app SEAL::seal)
target_link_libraries(test SEAL::seal)

set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
