cmake_minimum_required(VERSION 3.12)
project(clamir LANGUAGES C CXX VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_SYSTEM_INCLUDE_PATH: ${CMAKE_SYSTEM_INCLUDE_PATH}")

# find_package(OSAL REQUIRED)
# find_package(PNET REQUIRED)

# set(Boost_USE_STATIC_LIBS   ON)
# find_package(Boost REQUIRED COMPONENTS)

# find_package(Eigen3 REQUIRED)

# find_package(ODB REQUIRED COMPONENTS sqlite)

# include(${ODB_USE_FILE})
include(FetchContent)

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

# file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# odb_compile(OdbCMake_SOURCES FILES ${OdbCMake_ODB_HEADERS} DB sqlite GENERATE_QUERY GENERATE_SESSION)

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
)

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${OSAL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}-obj PUBLIC ${PNET_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME}-obj PUBLIC -Wall -Wdeprecated -Wdeprecated-declarations -g)
target_compile_definitions(${PROJECT_NAME}-obj PUBLIC -DDEBUGGING_HOST=0 -DNIT_CLAMIR_DEBUGGING)

target_include_directories(${PROJECT_NAME}-obj PUBLIC ${BOOST_INCLUDE_DIRS})

set_property(TARGET ${PROJECT_NAME}-obj
    PROPERTY
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED true
)

configure_file(version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/nit/embedded/core/version.hpp @ONLY)

add_executable(${PROJECT_NAME} src/main.cpp)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED true
)

target_compile_options(${PROJECT_NAME} PUBLIC -g)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-obj)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_SOURCE_DIR}/share/*.conf)
install(FILES ${${PROJECT_NAME}_SOURCES} DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/clamir/drivers.conf.d)

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