cmake_minimum_required(VERSION 3.22)

message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")

if(CMAKE_VERSION VERSION_GREATER "3.30.0") 
    cmake_policy(SET CMP0167 NEW)
else()
endif()

project(clamir-linux-common LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(NIT_CLAMIR_ENABLE_TESTING "Enable Testing" OFF)
option(NIT_CLAMIR_ENABLE_DOCS "Enable Documentation" OFF)
option(NIT_CLAMIR_ENABLE_PROFINET "Enable Profinet Extensio" OFF)
option(NIT_CLAMIR_HOST_MOCKUP "Enable Host Mockup" OFF)
option(NIT_CLAMIR_BUILD_UTILS "Enable Host Mockup" ON)
option(NIT_CLAMIR_ENABLE_BLAS "Enable Host Mockup" OFF)
option(NIT_CLAMIR_DEBUGGING "Enable Debugging" ON)

message(STATUS "NIT_CLAMIR_HOST_MOCKUP: ${NIT_CLAMIR_HOST_MOCKUP}")
message(STATUS "NIT_CLAMIR_DEBUGGING: ${NIT_CLAMIR_DEBUGGING}")
message(STATUS "NIT_CLAMIR_ENABLE_BLAS: ${NIT_CLAMIR_ENABLE_BLAS}")

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please inll it")
  return ("Error - pkg-config not found")
endif()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_SYSTEM_INCLUDE_PATH: ${CMAKE_SYSTEM_INCLUDE_PATH}")

include(FetchContent)

set(SKIP_PORTABILITY_TEST YES CACHE INTERNAL "Turn off CEREAL Compatibility Tests")
set(SKIP_PERFORMANCE_COMPARISON YES CACHE INTERNAL "Turn off CEREAL Performance Tests")
set(BUILD_DOC NO CACHE INTERNAL "Turn off CEREAL Build Documentation")
set(BUILD_SANDBOX NO CACHE INTERNAL "Turn off CEREAL Sandbox Build")

find_package(Boost REQUIRED COMPONENTS system serialization)
# pkg_check_modules(popt REQUIRED IMPORTED_TARGET popt)
pkg_check_modules(dbus-cxx REQUIRED IMPORTED_TARGET dbus-cxx-2.0)

if(NIT_CLAMIR_ENABLE_BLAS)
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
endif()

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(${PROJECT_NAME}-obj ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OSAL_INCLUDE_DIRS} )

message(STATUS "OSAL_INCLUDE_DIRS: ${OSAL_INCLUDE_DIR}")

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${dbus-cxx_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME}-obj
    PUBLIC
        ${GSTREAMER_LIBRARIES}
        m
        ${Boost_LIBRARIES}
        # PkgConfig::popt
        PkgConfig::dbus-cxx
        # lockfile
)

if(NIT_CLAMIR_ENABLE_BLAS)
    target_link_libraries(${PROJECT_NAME}-obj PUBLIC Eigen3::Eigen)
endif()

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${OSAL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}-obj PUBLIC ${PNET_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME}-obj PUBLIC -Wall -Wdeprecated -Wdeprecated-declarations -g -O0)


if (NIT_CLAMIR_HOST_MOCKUP)
    
    target_compile_definitions(${PROJECT_NAME}-obj PUBLIC -DNIT_CLAMIR_HOST_MOCKUP)
endif()

if (NIT_CLAMIR_DEBUGGING)
    
    target_compile_definitions(${PROJECT_NAME}-obj PUBLIC -DNIT_CLAMIR_DEBUGGING)
endif()

if (NIT_CLAMIR_ENABLE_BLAS)
    
    target_compile_definitions(${PROJECT_NAME}-obj PUBLIC -DNIT_CLAMIR_ENABLE_BLAS)
endif()

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${BOOST_INCLUDE_DIRS})

set_property(TARGET ${PROJECT_NAME}-obj
    PROPERTY
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED true
)

configure_file(version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/version.hpp @ONLY)
configure_file(config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/config.hpp @ONLY)
# configure_file(share/${PROJECT_NAME}-ct.service.in ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}.service @ONLY)

configure_file(scripts/build.sh.in ${CMAKE_SOURCE_DIR}/build.sh @ONLY)
configure_file(scripts/debug.sh.in ${CMAKE_SOURCE_DIR}/debug.sh @ONLY)
configure_file(scripts/deploy.sh.in ${CMAKE_SOURCE_DIR}/deploy.sh @ONLY)
configure_file(scripts/launch-debug.sh.in ${CMAKE_SOURCE_DIR}/launch-debug.sh @ONLY)

add_library(${PROJECT_NAME} SHARED src/shared.cpp)

set_property(TARGET ${PROJECT_NAME}-obj
    PROPERTY
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED true
)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED true
)

target_compile_options(${PROJECT_NAME} PUBLIC -g)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-obj)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

file(GLOB_RECURSE ${PROJECT_NAME}_CONF_SOURCES ${CMAKE_SOURCE_DIR}/share/*)

install(FILES ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}.service DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/systemd/system)
install(FILES ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/avahi/services)

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
