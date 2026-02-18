# template_project_ai

Sistema de revisión y corrección automática de código en PRs usando GitHub Copilot.

## 🎯 Descripción

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

## 🏗️ Arquitectura del Sistema

### 1. Build Workflow (`build.yml`)
- Se ejecuta en cada push/PR a las ramas `main` y `develop`
- Compila el proyecto en Windows y Ubuntu
- Detecta errores de compilación, warnings, y fallos de tests

### 2. Autofix Request Workflow (`copilot_autofix_request.yml`)
- Se dispara automáticamente cuando el workflow de Build **falla**
- Identifica los jobs fallidos y sus logs
- Publica un comentario en el PR mencionando a @copilot con:
  - Descripción del fallo
  - Links directos a los logs de CI
  - Instrucciones para corregir el problema

### 3. Flujo de Trabajo Completo

```mermaid
graph LR
    A[Push a PR] --> B[Build CI]
    B -->|✅ Éxito| C[PR aprobado]
    B -->|❌ Fallo| D[Copilot Autofix Request]
    D --> E[@copilot analiza logs]
    E --> F[Copilot corrige código]
    F --> G[Push a misma rama]
    G --> B
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

### Para Desarrolladores

1. **Crea una rama y haz cambios:**
   ```bash
   git checkout -b mi-feature
   # Haz tus cambios...
   git add .
   git commit -m "Add new feature"
   git push origin mi-feature
   ```

2. **Abre un Pull Request**
   - El CI se ejecutará automáticamente
   
3. **Si el CI falla:**
   - El workflow `copilot_autofix_request` publicará un comentario
   - @copilot analizará los logs y encontrará la causa raíz
   - @copilot corregirá el código y hará push a tu rama
   - El CI volverá a ejecutarse automáticamente

### Para Mantenedores

Los workflows están configurados con los permisos necesarios:
- `actions: read` - Leer información de workflows
- `pull-requests: write` - Comentar en PRs
- `issues: write` - Crear comentarios

## 📝 Ejemplo de Código

El proyecto incluye un ejemplo de calculadora en C++ (`src/main.cpp`) que demuestra:
- Estructura básica de clases
- Manejo de errores (división por cero)
- Entrada/salida estándar

## 🔧 Construcción Local

### Requisitos
- CMake 3.27+
- Ninja
- Compilador C++ con soporte para C++17

### Compilar en Linux/macOS
```bash
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --config Release
./build/dummy_project_ai
```

### Compilar en Windows
```bash
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --config Release
build\dummy_project_ai.exe
```

## 📚 Configuración de Workflows

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

## 🤝 Contribuir

1. Fork el repositorio
2. Crea una rama de feature (`git checkout -b feature/amazing-feature`)
3. Commit tus cambios (`git commit -m 'Add amazing feature'`)
4. Push a la rama (`git push origin feature/amazing-feature`)
5. Abre un Pull Request
6. ¡Deja que el sistema automatizado te ayude si algo falla!

## 📄 Licencia

Este es un proyecto de ejemplo para demostrar capacidades de automatización con GitHub Actions y Copilot.
