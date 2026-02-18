# DummyLux

A minimal C++20 application built with CMake and vcpkg.

## Requirements

- **CMake** >= 3.21
- **C++20** compatible compiler (MSVC 2019+, GCC 10+, Clang 10+)
- **vcpkg** (optional, for dependency management)
- **Ninja** (recommended build tool)

## Building on Linux

### Using Ninja (Recommended)

```bash
# Configure the project
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run
./build/DummyLux
```

### Using Unix Makefiles

```bash
# Configure the project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run
./build/DummyLux
```

### With vcpkg

If you have vcpkg installed:

```bash
# Set VCPKG_ROOT environment variable
export VCPKG_ROOT=/path/to/vcpkg

# Configure with vcpkg toolchain
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build

# Run
./build/DummyLux
```

## Building on Windows

### Using MSVC with Ninja

```powershell
# Configure the project
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
.\build\DummyLux.exe
```

### Using MSVC with Visual Studio Generator

```powershell
# Configure the project
cmake -S . -B build -A x64 -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
.\build\Release\DummyLux.exe
```

### With vcpkg on Windows

If you have vcpkg installed:

```powershell
# Using environment variable (if VCPKG_ROOT is set)
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

# Build
cmake --build build --config Release

# Run
.\build\DummyLux.exe
```

## Expected Output

When you run the executable, it should print:

```
DummyLux starting...
```

## CI/CD

The project includes GitHub Actions workflows that automatically build and test on:
- **Windows** (windows-latest with MSVC)
- **Ubuntu** (ubuntu-latest with GCC)

## Project Structure

```
.
├── src/
│   └── main.cpp          # Main application entry point
├── CMakeLists.txt        # CMake build configuration
├── vcpkg.json            # vcpkg manifest for dependencies
├── .github/
│   └── workflows/
│       └── build.yml     # CI/CD configuration
└── README.md             # This file
```

## Development

This is Phase 01 of the DummyLux project, establishing the basic infrastructure:
- C++20 executable
- CMake build system
- vcpkg manifest mode (for future dependencies)
- Cross-platform CI (Windows & Ubuntu)

Future phases will add OpenGL rendering, ImGui, and other features.
