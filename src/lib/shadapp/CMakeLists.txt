include_directories(${QT_INCLUDE_DIRS})

file(GLOB LIBSHADAPP_SRC "*.cpp")

add_library(shadapp0 SHARED
  ${LIBSHADAPP_SRC}
)

target_link_libraries(shadapp0
  ${QT_LIBRARIES}
)

set_target_properties(shadapp0
  PROPERTIES
  VERSION ${CPACK_PACKAGE_VERSION}
  SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
)

install(
  TARGETS shadapp0
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
