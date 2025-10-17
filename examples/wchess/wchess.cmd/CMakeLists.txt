if (WHISPER_SDL2 OR WHISPER_SDL3)
    set(TARGET wchess)
    add_executable(${TARGET} wchess.cmd.cpp)

    include(DefaultTargetOptions)

    target_link_libraries(${TARGET} PRIVATE wchess-core common-sdl ${CMAKE_THREAD_LIBS_INIT})
endif ()
