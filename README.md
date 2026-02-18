# DummyLux

Sistema de revisión y corrección automática de código en PRs usando GitHub Copilot.

## Requirements

Este proyecto demuestra un sistema automatizado que:
- **Detecta fallos** en Pull Requests cuando el CI falla
- **Invoca automáticamente** al agente @copilot para revisar y corregir el código
- **Publica las correcciones** directamente en el mismo PR

## 📖 Documentación

### 📚 Guías Principales

- **[📋 Referencia Rápida](docs/QUICK_REFERENCE.md)** - ⚡ Respuesta directa a "¿Cómo llamar a Copilot desde un Action?"
- **[📘 Guía de Uso Completa](docs/COPILOT_AGENT_USAGE.md)** - Tutorial detallado de configuración y personalización
- **[📝 Ejemplo del Flujo de Trabajo](docs/AUTOFIX_EXAMPLE.md)** - Demostración paso a paso del sistema
- **[🧪 Escenarios de Prueba](docs/TEST_SCENARIOS.md)** - Casos de prueba para validar el sistema

### 🎯 Según tu Necesidad

| Si necesitas... | Lee esto |
|----------------|----------|
| 🚀 **Respuesta rápida** | [Referencia Rápida](docs/QUICK_REFERENCE.md) |
| 🔧 **Implementar el sistema** | [Guía de Uso - Configuración](docs/COPILOT_AGENT_USAGE.md#configuración-paso-a-paso) |
| 🐛 **Resolver un problema** | [Guía de Uso - Troubleshooting](docs/COPILOT_AGENT_USAGE.md#solución-de-problemas) |
| 📐 **Entender cómo funciona** | [Ejemplo del Flujo](docs/AUTOFIX_EXAMPLE.md) |
| ✅ **Validar la implementación** | [Escenarios de Prueba](docs/TEST_SCENARIOS.md) |

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

## 🚀 Inicio Rápido

### ¿Cómo llamar al agente Copilot desde un Action?

**Respuesta corta:** Cuando tu workflow de CI falla, el sistema automáticamente invoca a @copilot en el PR.

**Pasos:**

1. **Configura los workflows** (ya incluidos en este repo):
   - `.github/workflows/build.yml` - Tu CI/build normal
   - `.github/workflows/copilot_autofix_request.yml` - Invoca a Copilot cuando falla

2. **Abre un PR y espera:**
   - Si el CI falla, Copilot será invocado automáticamente
   - Copilot analizará los logs y corregirá el código
   - Los cambios se publicarán en el mismo PR

3. **Lee la [Guía de Uso Completa](docs/COPILOT_AGENT_USAGE.md)** para más detalles

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

### build.yml
- **Ubicación:** `.github/workflows/build.yml`
- **Trigger:** `push` y `pull_request` a `main`, `develop`
- **Plataformas:** Windows y Ubuntu
- **Propósito:** Ejecutar CI/build y detectar errores

### copilot_autofix_request.yml
- **Ubicación:** `.github/workflows/copilot_autofix_request.yml`
- **Trigger:** `workflow_run` cuando Build falla (`conclusion: failure`)
- **Acción:** Publica comentario con @copilot en PRs relacionados
- **Permisos necesarios:**
  - `actions: read` - Leer información de workflows
  - `pull-requests: write` - Comentar en PRs
  - `issues: write` - Crear comentarios
  - `contents: read` - Leer código del repositorio

### Mensaje enviado a Copilot

El workflow envía automáticamente un comentario como:

```
@copilot

🔴 CI falló en [workflow run URL]

Jobs fallidos:
- build-ubuntu (failure) — [Ver logs]
- build-windows (failure) — [Ver logs]

Por favor:
1. Analiza los logs
2. Corrige el error
3. Push a la misma rama
```

**[Ver guía completa de configuración →](docs/COPILOT_AGENT_USAGE.md#configuración-paso-a-paso)**

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
