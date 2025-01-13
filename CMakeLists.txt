cmake_minimum_required(VERSION 3.12)
project(clamir LANGUAGES C CXX VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

include(FetchContent)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(${PROJECT_NAME}-obj ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME}-obj
    PUBLIC
        ${GSTREAMER_LIBRARIES}
        m
        ${BOOST_LIBRARIES}
)

target_compile_options(${PROJECT_NAME}-obj PUBLIC -g)

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

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-obj)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

install(FILES share/*.conf DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/clamir/drivers.conf.d)

if(NIT_CLAMIR_ENABLE_RAW)
endif()

if(NIT_CLAMIR_ENABLE_PROFINET)
    # find_package(NetSNMP REQUIRED)
    # find_package(NetSNMPAgent REQUIRED)
    find_package(Osal REQUIRED)
    message(STATUS "Added Osal")
    find_package(Profinet REQUIRED)
    message(STATUS "Added Profinet")
    include_directories(/usr/local/include)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            osal
            profinet
            # NetSNMP::NetSNMP
            # NetSNMP::NetSNMPAgent
    )
    #target_include_directories(${PROJECT_NAME} PUBLIC Osal Profinet)
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