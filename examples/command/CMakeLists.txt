if (WHISPER_SDL2 OR WHISPER_SDL3)
    set(TARGET whisper-command)
    add_executable(${TARGET} command.cpp)

    include(DefaultTargetOptions)

    target_link_libraries(${TARGET} PRIVATE common common-sdl whisper ${CMAKE_THREAD_LIBS_INIT})

    if (WHISPER_SDL3)
        target_compile_definitions(${TARGET} PRIVATE WHISPER_SDL3)
    endif()

    install(TARGETS ${TARGET} RUNTIME)
endif ()
