cmake_minimum_required(VERSION 3.12)
project(clamir C CXX)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} m)