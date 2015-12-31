cmake_minimum_required(VERSION 2.8)

project(LACADIA)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

find_package(OpenGL REQUIRED)

add_subdirectory(external/lodepng)
add_subdirectory(src/util)
add_subdirectory(src/game)
add_subdirectory(src/players)
add_subdirectory(src/enemies)

set(GLEW_RUNTIME ${LACADIA_SOURCE_DIR}/external/glew/bin/Release/Win32/glew32.dll)
set(VERTEX_SHADER ${LACADIA_SOURCE_DIR}/shaders/vertex_shader.glsl)
set(FRAGMENT_SHADER ${LACADIA_SOURCE_DIR}/shaders/fragment_shader.glsl)

list(APPEND LACADIA_SOURCES
    src/main.c

    $<TARGET_OBJECTS:lodepng>
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:game>
    $<TARGET_OBJECTS:players>
    $<TARGET_OBJECTS:enemies>
)

add_executable(lacadia ${LACADIA_SOURCES})
target_include_directories(lacadia
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
)
target_link_libraries(lacadia
    ${OPENGL_LIBRARIES}
    ${LACADIA_SOURCE_DIR}/external/glew/lib/Release/Win32/glew32.lib
    ${LACADIA_SOURCE_DIR}/external/glfw3/lib-vc2013/glfw3.lib
)
add_custom_command(TARGET lacadia
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VERTEX_SHADER} $<TARGET_FILE_DIR:lacadia>
)
add_custom_command(TARGET lacadia
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FRAGMENT_SHADER} $<TARGET_FILE_DIR:lacadia>
)
add_custom_command(TARGET lacadia
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_RUNTIME} $<TARGET_FILE_DIR:lacadia>
)