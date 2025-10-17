if (WHISPER_SDL2 OR WHISPER_SDL3)
    set(TARGET wchess)
    add_executable(${TARGET} wchess.cmd.cpp)

    include(DefaultTargetOptions)

    target_link_libraries(${TARGET} PRIVATE wchess-core common-sdl ${CMAKE_THREAD_LIBS_INIT})

    if (WHISPER_SDL3)
        target_compile_definitions(${TARGET} PRIVATE WHISPER_SDL3)
    endif()
endif ()
