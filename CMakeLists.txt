cmake_minimum_required(VERSION 3.12)
project(clamir LANGUAGES C CXX VERSION 0.1.0)

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

include(FetchContent)

pkg_check_modules(GSTREAMER gstreamer-1.0>=1.6 gstreamer-video-1.0>=1.6 REQUIRED)
include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${PROJECT_NAME}-obj ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-obj PUBLIC ${GSTREAMER_LIBRARIES})
add_compile_definitions(DEMOSEMAFORO)
configure_file(version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/version.hpp @ONLY)

add_executable(${PROJECT_NAME} src/clamir.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-obj)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(NIT_CLAMIR_ENABLE_RAW)
endif()

if(NIT_CLAMIR_ENABLE_PROFINET)
endif()

if(NIT_CLAMIR_ENABLE_RAW)
endif()

message(STATUS "NIT_CLAMIR_ENABLE_TESTING: ${NIT_CLAMIR_ENABLE_TESTING}")
if(NIT_CLAMIR_ENABLE_TESTING)
    enable_testing()
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b514bdc898e2951020cbdca1304b75f5950d1f59 # release-1.15.2
    )
      
    FetchContent_MakeAvailable(googletest)
    
    include(GoogleTest)
    add_subdirectory(test)
endif()