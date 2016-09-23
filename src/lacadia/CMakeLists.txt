add_subdirectory(abilities)
add_subdirectory(effects)
add_subdirectory(enemies)
add_subdirectory(players)

list(APPEND LACADIA_SOURCES
    coin.c
    coin.h
    main.c

    ${EXTERNAL_OBJECT_LIBRARIES}
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:graphics>
    $<TARGET_OBJECTS:engine>
    $<TARGET_OBJECTS:players>
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
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LACADIA_SOURCE_DIR}/assets $<TARGET_FILE_DIR:lacadia>/assets
)

if(WIN32)
add_custom_command(TARGET lacadia
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${LACADIA_SOURCE_DIR}/external/glew/bin/Release/Win32/glew32.dll $<TARGET_FILE_DIR:lacadia>
)
endif(WIN32)
