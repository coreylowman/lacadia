cmake_minimum_required(VERSION 2.8)

project(LACADIA)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

find_package(OpenGL REQUIRED)

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
    ${LACADIA_SOURCE_DIR}/external/glfw/include
    ${LACADIA_SOURCE_DIR}/external/glew/include
)

if(WIN32)
    list(APPEND LACADIA_INCLUDE_DIRS
        ${LACADIA_SOURCE_DIR}/external/dirent/include
    )
endif(WIN32)


add_subdirectory(external/lodepng)
add_subdirectory(external/freetype-2.6.5)
add_subdirectory(external/glfw)
add_subdirectory(external/glew/build/cmake)

set(EXTERNAL_LIBRARIES
    ${OPENGL_LIBRARIES}
    glew_s
    glfw
)

set(EXTERNAL_OBJECT_LIBRARIES
    $<TARGET_OBJECTS:freetype>
    $<TARGET_OBJECTS:lodepng>
)

add_subdirectory(src/engine)
add_subdirectory(src/lacadia)
