cmake_minimum_required(VERSION 3.14)
project(conway C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
)
if(APPLE)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      "-framework IOKit"
      "-framework Cocoa"
      "-framework OpenGL"
  )
endif()
