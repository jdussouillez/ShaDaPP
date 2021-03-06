cmake_minimum_required(VERSION 2.8.8)

project(SHADAPP CXX)

include(CPackConfig.cmake)
include(GNUInstallDirs)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtNetwork QtXml QtXmlPatterns)
include(${QT_USE_FILE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBLZ4 REQUIRED liblz4)

add_definitions(-Wall -Wextra -g -O0)
add_definitions(-std=c++11)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib/shadapp)
add_subdirectory(bin)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/shadapp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/share/shadapp"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/shadapp"
)
