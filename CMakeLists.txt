cmake_minimum_required(VERSION 2.8)

project(LACADIA)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

find_package(OpenGL REQUIRED)

if(UNIX)
    find_package(GLEW REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
elseif(WIN32)
    set(GLEW_INCLUDE_DIRS ${LACADIA_SOURCE_DIR}/external/glew/include)
    set(GLEW_LIBRARIES ${LACADIA_SOURCE_DIR}/external/glew/lib/Release/Win32/glew32.lib)

    set(GLFW_INCLUDE_DIRS ${LACADIA_SOURCE_DIR}/external/glfw3/include)
    set(GLFW_LIBRARIES ${LACADIA_SOURCE_DIR}/external/glfw3/lib-vc2013/glfw3.lib)
endif(UNIX)

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "\t\t${_variableName}=${${_variableName}}")
# endforeach()

# for freetype cmake
set(SKIP_INSTALL_HEADERS TRUE)
set(SKIP_INSTALL_LIBRARIES TRUE)
set(SKIP_INSTALL_ALL TRUE)
set(WITH_ZLIB OFF)
set(WITH_BZip2 OFF)
set(WITH_PNG OFF)
set(WITH_HarfBuzz OFF)

list(APPEND LACADIA_INCLUDE_DIRS
    ${LACADIA_SOURCE_DIR}/src
    ${LACADIA_SOURCE_DIR}/external/lodepng/include
    ${LACADIA_SOURCE_DIR}/external/freetype-2.6.5/include
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

add_subdirectory(external/lodepng)
add_subdirectory(external/freetype-2.6.5)
add_subdirectory(src/util)
add_subdirectory(src/components)
add_subdirectory(src/game)
add_subdirectory(src/players)
add_subdirectory(src/collectables)
add_subdirectory(src/enemies)
add_subdirectory(src/effects)
add_subdirectory(src/abilities)

list(APPEND LACADIA_SOURCES
    src/main.c

    $<TARGET_OBJECTS:freetype>
    $<TARGET_OBJECTS:lodepng>
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:components>
    $<TARGET_OBJECTS:game>
    $<TARGET_OBJECTS:players>
    $<TARGET_OBJECTS:collectables>
    $<TARGET_OBJECTS:enemies>
    $<TARGET_OBJECTS:effects>
    $<TARGET_OBJECTS:abilities>
)

add_executable(lacadia ${LACADIA_SOURCES})
target_include_directories(lacadia
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
target_link_libraries(lacadia
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)
if(UNIX)
target_link_libraries(lacadia
    m
    Xxf86vm
    pthread
    dl
    X11
    Xi
    Xinerama
    Xrandr
    Xcursor
)
endif(UNIX)

add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LACADIA_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:lacadia>/shaders
)

add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LACADIA_SOURCE_DIR}/assets $<TARGET_FILE_DIR:lacadia>/assets
)

if(WIN32)
add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${LACADIA_SOURCE_DIR}/external/glew/bin/Release/Win32/glew32.dll $<TARGET_FILE_DIR:lacadia>
)
endif(WIN32)
