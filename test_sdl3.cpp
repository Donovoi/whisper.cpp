#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <iostream>

int main() {
    std::cout << "Testing SDL3 integration...\n";
    
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return 1;
    }
    
    std::cout << "SDL3 initialized successfully!\n";
    std::cout << "SDL Version: " << SDL_GetRevision() << "\n";
    
    // Try to enumerate audio recording devices
    int count = 0;
    SDL_AudioDeviceID* devices = SDL_GetAudioRecordingDevices(&count);
    
    std::cout << "Found " << count << " audio recording device(s)\n";
    
    if (devices) {
        for (int i = 0; i < count; i++) {
            const char* name = SDL_GetAudioDeviceName(devices[i]);
            std::cout << "  Device " << i << ": " << (name ? name : "Unknown") << "\n";
        }
        SDL_free(devices);
    }
    
    SDL_Quit();
    std::cout << "\nSDL3 test completed successfully!\n";
    return 0;
}
