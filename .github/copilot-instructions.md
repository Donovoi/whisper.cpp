# Copilot Instructions for whisper.cpp

This codebase provides a high-performance C/C++ implementation of OpenAI's Whisper automatic speech recognition (ASR) model.

## Architecture Overview

**Core Components:**

- `src/whisper.cpp` + `include/whisper.h`: Main inference engine (~9000 lines, contains entire model implementation)
- `ggml/`: Machine learning tensor library (similar to PyTorch tensors but C-optimized)
- `examples/`: CLI tools, server, and platform-specific implementations
- `models/`: Model conversion scripts and download utilities

**Key Design Principles:**

- Zero runtime memory allocations after initialization
- Single-file model format (ggml) containing weights + vocabulary + mel filters
- Hardware-optimized backends (Metal, CUDA, OpenVINO, Core ML)
- C-style API for easy embedding in any language

## Critical Build & Development Patterns

**Standard Build Flow:**

```bash
cmake -B build -DWHISPER_CUDA=1  # Enable GPU support
cmake --build build -j --config Release

# Quick test with model download
make base.en  # Downloads model + runs on samples
```

**Model Management:**

- Models stored in custom `ggml` format (not PyTorch .pt files)
- Use `./models/download-ggml-model.sh base.en` to get pre-converted models
- Model sizes: tiny (75MB) → base (142MB) → small (466MB) → medium (1.5GB) → large (2.9GB)
- Quantized variants available (e.g., `large-v3-q5_0.bin`) for reduced memory

**Hardware Acceleration Flags:**

```cmake
-DWHISPER_CUDA=1        # NVIDIA GPU
-DWHISPER_METAL=1       # Apple Silicon GPU
-DWHISPER_COREML=1      # Apple Neural Engine
-DWHISPER_OPENVINO=1    # Intel optimization
-DGGML_BLAS=1           # CPU acceleration via OpenBLAS
```

## API Usage Patterns

**Basic C API Flow (see `examples/cli/cli.cpp`):**

```cpp
// 1. Initialize context
whisper_context_params cparams = whisper_context_default_params();
whisper_context * ctx = whisper_init_from_file_with_params("models/ggml-base.en.bin", cparams);

// 2. Prepare parameters
whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
wparams.language = "en";
wparams.print_timestamps = true;

// 3. Process audio (16kHz float32 PCM)
whisper_full(ctx, wparams, pcmf32.data(), pcmf32.size());

// 4. Extract results
int n_segments = whisper_full_n_segments(ctx);
for (int i = 0; i < n_segments; ++i) {
    const char * text = whisper_full_get_segment_text(ctx, i);
    // Process transcription...
}
```

**Audio Input Requirements:**

- 16kHz sample rate (WHISPER_SAMPLE_RATE constant)
- Single channel (mono) float32 PCM data
- Use `read_audio_data()` from `common-whisper.h` for WAV files
- ffmpeg for format conversion: `ffmpeg -i input.mp3 -ar 16000 -ac 1 -c:a pcm_s16le output.wav`

## Testing & Quality Assurance

**Integration Tests:**

```bash
./tests/run-tests.sh base.en 4  # Run against reference transcriptions
make samples                    # Download test audio files
```

**Performance Benchmarking:**

```bash
./build/bin/whisper-bench -m models/ggml-base.en.bin  # Encoder performance
python3 scripts/bench.py -f samples/jfk.wav          # Comprehensive benchmarks
```

## Advanced Features

**Voice Activity Detection (VAD):**

```bash
# Download VAD model
./models/download-vad-model.sh silero-v5.1.2
# Use with whisper
./build/bin/whisper-cli --vad -vm models/ggml-silero-v5.1.2.bin -m models/ggml-base.en.bin -f audio.wav
```

**Real-time Streaming:**

- See `examples/stream/` for microphone input using SDL2/SDL3
- Build with `-DWHISPER_SDL3=ON` (recommended) or `-DWHISPER_SDL2=ON` (legacy)
- SDL3 provides improved audio APIs and performance
- For Ubuntu systems before October 2025, SDL3 must be built from source

**Server Deployment:**

- HTTP server with OpenAI-compatible API in `examples/server/`
- Supports multiple output formats: JSON, SRT, VTT, plain text

## Common Gotchas

1. **Model Format**: Always use `.bin` files from `models/download-ggml-model.sh`, not raw PyTorch files
2. **Audio Format**: Whisper is very sensitive to sample rate - must be exactly 16kHz
3. **Threading**: Use `wparams.n_threads = N` to control CPU usage, not CMake parallelism
4. **Memory**: Large models (medium+) require significant RAM; consider quantized versions
5. **GPU Memory**: CUDA/Metal acceleration needs model to fit in VRAM

## Platform-Specific Notes

**iOS/macOS**: Use Core ML acceleration (`-DWHISPER_COREML=1`) for best performance on Apple Silicon
**Android**: Pre-built examples in `examples/whisper.android/`
**WebAssembly**: Browser examples in `examples/whisper.wasm/`
**Windows**: Supports both MSVC and MinGW builds

When implementing new features, follow the pattern of other examples in `examples/` directory and use the common utilities in `examples/common-whisper.h` for audio processing.
