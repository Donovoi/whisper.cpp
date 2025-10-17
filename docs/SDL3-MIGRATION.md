# SDL2 to SDL3 Migration Guide for whisper.cpp

This document outlines the changes required for migrating from SDL2 to SDL3 in whisper.cpp.

## Overview

whisper.cpp now supports both SDL2 and SDL3 for real-time audio capture in examples like `whisper-stream`, `command`, `talk-llama`, and others. SDL3 is recommended for new projects due to its improved audio API and better cross-platform support.

## Building with SDL3

### Linux (Ubuntu/Debian)
```bash
# Install SDL3 (when available in package managers)
sudo apt-get install libsdl3-dev

# Build whisper.cpp with SDL3
cmake -B build -DWHISPER_SDL3=ON
cmake --build build -j --config Release
```

### macOS
```bash
# Install SDL3 via Homebrew
brew install sdl3

# Build whisper.cpp with SDL3
cmake -B build -DWHISPER_SDL3=ON
cmake --build build -j --config Release
```

### Windows (MSVC)
```powershell
# Download SDL3 development libraries from https://github.com/libsdl-org/SDL/releases
# Extract and set SDL3_DIR environment variable

# Build whisper.cpp with SDL3
cmake -B build -DWHISPER_SDL3=ON -DSDL3_DIR="C:\path\to\SDL3\cmake"
cmake --build build --config Release
```

## Key API Changes

### Audio Device Initialization

**SDL2:**
```cpp
SDL_Init(SDL_INIT_AUDIO);
SDL_GetNumAudioDevices(SDL_TRUE);  // Get capture devices
const char* name = SDL_GetAudioDeviceName(i, SDL_TRUE);
```

**SDL3:**
```cpp
SDL_Init(SDL_INIT_AUDIO);  // Audio subsystem auto-initializes
int count;
SDL_AudioDeviceID* devices = SDL_GetAudioRecordingDevices(&count);
const char* name = SDL_GetAudioDeviceName(devices[i]);
SDL_free(devices);
```

### Audio Device Opening

**SDL2:**
```cpp
SDL_AudioSpec requested, obtained;
// Configure requested spec...
SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
    device_name, SDL_TRUE, &requested, &obtained, 0);
```

**SDL3:**
```cpp
SDL_AudioSpec spec = { SDL_AUDIO_F32, 1, sample_rate };
SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
    SDL_AUDIO_DEVICE_DEFAULT_RECORDING, &spec);

// Query obtained format
SDL_AudioSpec obtained;
SDL_GetAudioDeviceFormat(dev, &obtained, NULL);
```

### Audio Playback Control

**SDL2:**
```cpp
SDL_PauseAudioDevice(dev, 0);  // Resume (0 = unpause)
SDL_PauseAudioDevice(dev, 1);  // Pause (1 = pause)
```

**SDL3:**
```cpp
SDL_ResumeAudioDevice(dev);    // Resume
SDL_PauseAudioDevice(dev);     // Pause
```

### Audio Device Cleanup

**SDL2 & SDL3:**
```cpp
SDL_CloseAudioDevice(dev);  // Same in both versions
```

## Compatibility Layer

The codebase maintains compatibility with both SDL2 and SDL3 through preprocessor directives:

```cpp
#ifdef WHISPER_SDL3
    // SDL3-specific code
    SDL_AudioDeviceID* devices = SDL_GetAudioRecordingDevices(&count);
#else
    // SDL2-specific code
    int count = SDL_GetNumAudioDevices(SDL_TRUE);
#endif
```

## Migration Checklist

### For New Projects:
- ✅ Use `-DWHISPER_SDL3=ON` for new builds
- ✅ Install SDL3 development libraries
- ✅ Update documentation to reference SDL3

### For Existing SDL2 Projects:
- ✅ SDL2 continues to work with no changes required
- ✅ Both SDL2 and SDL3 can coexist in the build system
- ✅ Migrate at your own pace - no forced upgrade

### For Contributors:
- ✅ Test changes with both `-DWHISPER_SDL2=ON` and `-DWHISPER_SDL3=ON`
- ✅ Update `common-sdl.cpp` and `common-sdl.h` for API changes
- ✅ Maintain backwards compatibility where possible

## Known Issues & Limitations

### SDL3 Status (as of October 2025):
- SDL3 is in active development
- API may still be subject to changes
- Not all package managers have SDL3 yet

### Workarounds:
- Build SDL3 from source if not available in package managers
- Continue using SDL2 for production until SDL3 is stable
- Monitor SDL3 releases at: https://github.com/libsdl-org/SDL/releases

## Testing

Test both SDL2 and SDL3 builds:

```bash
# Test SDL2
cmake -B build-sdl2 -DWHISPER_SDL2=ON
cmake --build build-sdl2 -j
./build-sdl2/bin/whisper-stream -m models/ggml-base.en.bin

# Test SDL3
cmake -B build-sdl3 -DWHISPER_SDL3=ON
cmake --build build-sdl3 -j
./build-sdl3/bin/whisper-stream -m models/ggml-base.en.bin
```

## Additional Resources

- [SDL3 Migration Guide](https://github.com/libsdl-org/SDL/blob/main/docs/README-migration.md)
- [SDL3 Audio Documentation](https://wiki.libsdl.org/SDL3/CategoryAudio)
- [whisper.cpp Examples](../examples/)

## Support

For issues related to SDL2/SDL3 in whisper.cpp:
- Check existing issues: https://github.com/ggml-org/whisper.cpp/issues
- Create new issue with `[SDL3]` prefix
- Include build logs and SDL version information
