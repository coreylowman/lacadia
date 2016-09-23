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
    ${LACADIA_SOURCE_DIR}/external/dirent/include
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)


add_subdirectory(external/lodepng)
add_subdirectory(external/freetype-2.6.5)

set(EXTERNAL_OBJECT_LIBRARIES
    $<TARGET_OBJECTS:freetype>
    $<TARGET_OBJECTS:lodepng>
)

add_subdirectory(src/engine)
add_subdirectory(src/lacadia)
