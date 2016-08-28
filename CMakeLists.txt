cmake_minimum_required(VERSION 2.8)

project(LACADIA)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

find_package(OpenGL REQUIRED)

add_subdirectory(external/lodepng)
add_subdirectory(src/util)
add_subdirectory(src/components)
add_subdirectory(src/game)
add_subdirectory(src/players)
add_subdirectory(src/enemies)
add_subdirectory(src/effects)
add_subdirectory(src/abilities)

set(GLEW_RUNTIME ${LACADIA_SOURCE_DIR}/external/glew/bin/Release/Win32/glew32.dll)
set(VERTEX_SHADER ${LACADIA_SOURCE_DIR}/shaders/vertex_shader.glsl)
set(FRAGMENT_SHADER ${LACADIA_SOURCE_DIR}/shaders/fragment_shader.glsl)
set(LINE_VERTEX_SHADER ${LACADIA_SOURCE_DIR}/shaders/line_vertex_shader.glsl)
set(LINE_FRAGMENT_SHADER ${LACADIA_SOURCE_DIR}/shaders/line_fragment_shader.glsl)

list(APPEND LACADIA_SOURCES
    src/main.c

    $<TARGET_OBJECTS:lodepng>
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:components>
    $<TARGET_OBJECTS:game>
    $<TARGET_OBJECTS:players>
    $<TARGET_OBJECTS:enemies>
    $<TARGET_OBJECTS:effects>
    $<TARGET_OBJECTS:abilities>
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
)
if(MSVC14)
message("Using VS2015 version of glfw3")
target_link_libraries(lacadia
    ${LACADIA_SOURCE_DIR}/external/glfw3/lib-vc2015/glfw3.lib
)
elseif(MSVC12)
message("Using VS2013 version of glfw3")
target_link_libraries(lacadia
    ${LACADIA_SOURCE_DIR}/external/glfw3/lib-vc2013/glfw3.lib
)
endif()

add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LACADIA_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:lacadia>/shaders
)
add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LACADIA_SOURCE_DIR}/assets $<TARGET_FILE_DIR:lacadia>/assets
)
add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_RUNTIME} $<TARGET_FILE_DIR:lacadia>
)
