include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${QT_INCLUDE_DIR})

file(GLOB SHADAPP_SRC "*.cpp")

add_executable(shadapp ${SHADAPP_SRC})

target_link_libraries(shadapp
  shadapp0
)

install(
  TARGETS shadapp
  RUNTIME DESTINATION shadapp
)
