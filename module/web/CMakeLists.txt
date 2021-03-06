cmake_minimum_required(VERSION 3.13)
project(KingtousProject)
message(STATUS "configuring KingtousProject-Web模块")
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/unknown)
set(LOCATION "lib")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SSLLib openssl)
message(STATUS "found ssl lib in ${SSLLib_INCLUDE_DIRS}")
message(STATUS "found ssl lib in ${SSLLib_LIBRARY_DIRS}")
include_directories(${SSLLib_INCLUDE_DIRS})
link_directories(${SSLLib_LIBRARY_DIRS})

# Boost
message(STATUS "查找Boost框架")
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

if (APPLE)
    message(STATUS "macOS detected")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/mac)
    #    set(CMAKE_C_FLAGS  "-fPIC")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-fPIC")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/linux)
    message(STATUS "unix detected")
endif ()

#aux_source_directory(src SRC)

add_library(kweb STATIC src/web.hpp src/web.cpp)

add_executable(webdemo src/webdemo.cpp src/web.hpp src/web.cpp)

add_executable(serverdemo src/serverdemo.cpp src/web.hpp src/web.cpp)

target_link_libraries(webdemo Boost::filesystem Boost::system pthread ${SSLLib_LIBRARIES} ksql kjson  mysqlcppconn )

target_link_libraries(serverdemo Boost::filesystem Boost::system pthread ${SSLLib_LIBRARIES} ksql kjson mysqlcppconn )

if(WIN32)
    target_link_libraries(webdemo wsock32 ws2_32)

    target_link_libraries(serverdemo wsock32 ws2_32)

endif()

install(TARGETS kweb
        ARCHIVE DESTINATION ${LOCATION}
        LIBRARY DESTINATION ${LOCATION}
        )