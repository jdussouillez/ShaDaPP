include_directories(${QT_INCLUDE_DIRS})

set(SHADAPP_MOC_FILES
  "${CMAKE_SOURCE_DIR}/include/shadapp/fs/FileWatcherWorker.h"
  "${CMAKE_SOURCE_DIR}/include/shadapp/fs/FileWatcher.h"
  "${CMAKE_SOURCE_DIR}/include/shadapp/Network.h"
  "${CMAKE_SOURCE_DIR}/include/shadapp/fs/Device.h"
)
qt4_wrap_cpp(LIBSHADAPP_MOC ${SHADAPP_MOC_FILES})

file(GLOB LIBSHADAPP_SRC "*.cpp")
file(GLOB LIBSHADAPP_CONFIG_SRC "config/*.cpp")
file(GLOB LIBSHADAPP_DATA_SRC "data/*.cpp")
file(GLOB LIBSHADAPP_FS_SRC "fs/*.cpp")
file(GLOB LIBSHADAPP_PROTOCOL_SRC "protocol/*.cpp")

add_library(shadapp0 SHARED
  ${LIBSHADAPP_SRC}
  ${LIBSHADAPP_CONFIG_SRC}
  ${LIBSHADAPP_DATA_SRC}
  ${LIBSHADAPP_FS_SRC}
  ${LIBSHADAPP_PROTOCOL_SRC}
  ${LIBSHADAPP_MOC}
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
