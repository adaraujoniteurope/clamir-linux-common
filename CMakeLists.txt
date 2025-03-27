cmake_minimum_required(VERSION 3.5)
project(clamir-linux-daemon LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


option(NIT_CLAMIR_ENABLE_TESTING "Enable Testing" NO)
option(NIT_CLAMIR_ENABLE_DOCS "Enable Documentation" NO)
option(NIT_CLAMIR_ENABLE_PROFINET "Enable Profinet Extensio" NO)
option(NIT_CLAMIR_HOST_MOCKUP "Enable Host Mockup" true)

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_SYSTEM_INCLUDE_PATH: ${CMAKE_SYSTEM_INCLUDE_PATH}")

include(FetchContent)

set(SKIP_PORTABILITY_TEST YES CACHE INTERNAL "Turn of CEREAL Compatibility Tests")

FetchContent_Declare(
      cereal
      GIT_REPOSITORY https://github.com/USCiLab/cereal
      GIT_TAG v1.3.0)

FetchContent_GetProperties(cereal)

if (NOT cereal_POPULATED)
    FetchContent_Populate(cereal)
    add_subdirectory(
    ${cereal_SOURCE_DIR}
    ${cereal_BINARY_DIR}
    EXCLUDE_FROM_ALL)
    add_library(cereal::cereal ALIAS cereal )
endif ()

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(EIGEN_BUILD_TESTING OFF)
set(EIGEN_MPL2_ONLY ON)
set(EIGEN_BUILD_PKGCONFIG OFF)
set(EIGEN_BUILD_DOC OFF)

FetchContent_MakeAvailable(Eigen)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(${PROJECT_NAME}-obj ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OSAL_INCLUDE_DIRS} )

message(STATUS "OSAL_INCLUDE_DIRS: ${OSAL_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME}-obj
    PUBLIC
        # OSAL::OSAL
        # PNET::PNET
        ${GSTREAMER_LIBRARIES}
        m
        # ${Boost_LIBRARIES}
        cereal
        Eigen3::Eigen
)

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${OSAL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}-obj PUBLIC ${PNET_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME}-obj PUBLIC -Wall -Wdeprecated -Wdeprecated-declarations -g -O0)
target_compile_definitions(${PROJECT_NAME}-obj PUBLIC -DNIT_CLAMIR_DEBUGGING)

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${BOOST_INCLUDE_DIRS})

set_property(TARGET ${PROJECT_NAME}-obj
    PROPERTY
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED true
)

configure_file(version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/version.hpp @ONLY)
configure_file(config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/config.hpp @ONLY)
configure_file(share/${PROJECT_NAME}.service.in ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}.service @ONLY)

configure_file(scripts/build.sh.in ${CMAKE_SOURCE_DIR}/build.sh @ONLY)
configure_file(scripts/debug.sh.in ${CMAKE_SOURCE_DIR}/debug.sh @ONLY)
configure_file(scripts/deploy.sh.in ${CMAKE_SOURCE_DIR}/deploy.sh @ONLY)
configure_file(scripts/launch-debug.sh.in ${CMAKE_SOURCE_DIR}/launch-debug.sh @ONLY)

add_executable(${PROJECT_NAME} src/main.cpp)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED true
)

target_compile_options(${PROJECT_NAME} PUBLIC -g)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-obj)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

file(GLOB_RECURSE ${PROJECT_NAME}_CONF_SOURCES ${CMAKE_SOURCE_DIR}/share/*.json)

install(FILES ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}.service DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/systemd/system)

if(NIT_CLAMIR_ENABLE_RAW)
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