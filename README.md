# template_project_ai

Sistema de revisión y corrección automática de código en PRs.

## 🎯 Descripción

Este proyecto demuestra un sistema automatizado que:
- **Detecta fallos** en Pull Requests cuando el CI falla
- **Solicita automáticamente** a @copilot que revise y corrija el código
- **Publica las correcciones** en el mismo PR

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

## 🚀 Uso

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
- Ubicación: `.github/workflows/build.yml`
- Se ejecuta en: `push` y `pull_request` a `main`, `develop`
- Plataformas: Windows y Ubuntu

### copilot_autofix_request.yml
- Ubicación: `.github/workflows/copilot_autofix_request.yml`
- Trigger: `workflow_run` cuando Build completa con estado `failure`
- Acción: Publica comentario con @copilot en PRs relacionados

## 🤝 Contribuir

1. Fork el repositorio
2. Crea una rama de feature (`git checkout -b feature/amazing-feature`)
3. Commit tus cambios (`git commit -m 'Add amazing feature'`)
4. Push a la rama (`git push origin feature/amazing-feature`)
5. Abre un Pull Request
6. ¡Deja que el sistema automatizado te ayude si algo falla!

## 📄 Licencia

Este es un proyecto de ejemplo para demostrar capacidades de automatización con GitHub Actions y Copilot.
