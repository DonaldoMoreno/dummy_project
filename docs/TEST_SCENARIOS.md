# 🧪 Escenarios de Prueba para Copilot Autofix

Este documento describe varios escenarios de prueba para validar el funcionamiento del sistema de autofix con Copilot.

## 📋 Índice

- [Escenario 1: Error de Sintaxis Simple](#escenario-1-error-de-sintaxis-simple)
- [Escenario 2: Error de Compilación](#escenario-2-error-de-compilación)
- [Escenario 3: Test Fallido](#escenario-3-test-fallido)
- [Escenario 4: Warning de Compilador](#escenario-4-warning-de-compilador)
- [Escenario 5: Múltiples Errores](#escenario-5-múltiples-errores)

---

## Escenario 1: Error de Sintaxis Simple

### 🎯 Objetivo
Verificar que Copilot puede corregir errores de sintaxis básicos como puntos y coma faltantes.

### 📝 Pasos

1. **Crear una nueva rama:**
   ```bash
   git checkout -b test/syntax-error
   ```

2. **Introducir error de sintaxis en `src/main.cpp`:**
   ```cpp
   int main() {
       std::cout << "Hello" << std::endl  // Falta punto y coma
       return 0;
   }
   ```

3. **Commit y push:**
   ```bash
   git add src/main.cpp
   git commit -m "Test: Introduce syntax error"
   git push origin test/syntax-error
   ```

4. **Abrir PR** y esperar a que falle el CI

### ✅ Resultado Esperado
- El workflow `build.yml` falla
- El workflow `copilot_autofix_request.yml` se ejecuta
- Se publica un comentario en el PR con @copilot
- Copilot analiza el error y agrega el punto y coma faltante
- Copilot hace push a la misma rama
- El CI vuelve a ejecutarse y pasa

### 🔍 Verificación
```bash
# Verificar que el CI pasó
gh pr checks test/syntax-error

# Verificar que hay un comentario de Copilot
gh pr view test/syntax-error --comments

# Verificar que se agregó el punto y coma
git diff HEAD~1 src/main.cpp
```

---

## Escenario 2: Error de Compilación

### 🎯 Objetivo
Validar que Copilot puede corregir errores de compilación como includes faltantes.

### 📝 Pasos

1. **Crear rama:**
   ```bash
   git checkout -b test/missing-include
   ```

2. **Remover include necesario:**
   ```cpp
   // src/main.cpp
   // #include <iostream>  // Comentado para causar error
   #include <string>
   
   int main() {
       std::cout << "Hello" << std::endl;  // Error: cout no declarado
       return 0;
   }
   ```

3. **Push y abrir PR:**
   ```bash
   git add src/main.cpp
   git commit -m "Test: Remove iostream include"
   git push origin test/missing-include
   ```

### ✅ Resultado Esperado
- Build falla con error: `'cout' is not a member of 'std'`
- Copilot identifica el include faltante
- Copilot descomenta o agrega `#include <iostream>`
- CI pasa después de la corrección

---

## Escenario 3: Test Fallido

### 🎯 Objetivo
Verificar que Copilot puede corregir lógica incorrecta que causa fallos en tests.

### 📝 Pasos

1. **Crear rama:**
   ```bash
   git checkout -b test/failed-test
   ```

2. **Introducir bug en la lógica:**
   ```cpp
   // src/main.cpp
   int add(int a, int b) {
       return a - b;  // Error: debería ser a + b
   }
   ```

3. **Agregar test (si no existe):**
   ```cpp
   // tests/test_calculator.cpp
   #include <cassert>
   
   int add(int a, int b);
   
   int main() {
       assert(add(2, 3) == 5);  // Fallará con implementación actual
       return 0;
   }
   ```

4. **Push y abrir PR**

### ✅ Resultado Esperado
- Test falla
- Copilot identifica que `add` está usando `-` en lugar de `+`
- Copilot corrige la operación
- Tests pasan

---

## Escenario 4: Warning de Compilador

### 🎯 Objetivo
Validar que Copilot puede corregir warnings que pueden convertirse en errores.

### 📝 Pasos

1. **Modificar CMakeLists.txt para tratar warnings como errores:**
   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   ```

2. **Introducir código con warning:**
   ```cpp
   int main() {
       int unused_variable = 42;  // Warning: variable no usada
       return 0;
   }
   ```

3. **Push y abrir PR**

### ✅ Resultado Esperado
- Build falla por warning tratado como error
- Copilot identifica la variable no utilizada
- Copilot la remueve o la usa
- Build pasa

---

## Escenario 5: Múltiples Errores

### 🎯 Objetivo
Verificar que Copilot puede manejar múltiples errores simultáneos.

### 📝 Pasos

1. **Introducir varios errores:**
   ```cpp
   #include <iostream>
   
   int main() {
       int x = 10 / 0;  // Error 1: División por cero
       std::cout << y << std::endl;  // Error 2: y no declarado
       return 0  // Error 3: Falta punto y coma
   }
   ```

2. **Push y abrir PR**

### ✅ Resultado Esperado
- Build falla con múltiples errores
- Copilot identifica y corrige todos los errores:
  - Cambia división por cero a un divisor válido
  - Declara la variable `y` o usa `x`
  - Agrega el punto y coma faltante
- Build pasa

---

## 🛠️ Scripts de Automatización

### Script para ejecutar todos los escenarios

```bash
#!/bin/bash
# test_all_scenarios.sh

scenarios=(
  "syntax-error"
  "missing-include"
  "failed-test"
  "warning-error"
  "multiple-errors"
)

for scenario in "${scenarios[@]}"; do
  echo "Testing scenario: $scenario"
  git checkout -b "test/$scenario"
  
  # Aquí irían los cambios específicos de cada escenario
  # ...
  
  git push origin "test/$scenario"
  gh pr create --title "Test: $scenario" --body "Automated test scenario"
  
  echo "Waiting for CI to complete..."
  sleep 60
  
  # Verificar resultado
  gh pr checks "test/$scenario"
  
  echo "---"
done
```

### Script para limpiar branches de prueba

```bash
#!/bin/bash
# cleanup_test_branches.sh

git branch | grep "test/" | xargs git branch -D
git push origin --delete $(git branch -r | grep "origin/test/" | sed 's/origin\///')
```

---

## 📊 Métricas a Medir

Para cada escenario, registra:

1. **Tiempo hasta detección del error:** Desde push hasta comentario de Copilot
2. **Tiempo de corrección:** Desde comentario hasta commit de Copilot
3. **Número de intentos:** ¿Copilot corrigió en el primer intento?
4. **Calidad de la corrección:** ¿La solución es óptima?
5. **Falsos positivos:** ¿Copilot cambió código que no debía?

### Formato de Reporte

```markdown
| Escenario | Detección | Corrección | Intentos | Calidad | Notas |
|-----------|-----------|------------|----------|---------|-------|
| Sintaxis  | 2m 30s    | 1m 45s     | 1        | ✅ 5/5  | Perfecto |
| Include   | 2m 15s    | 2m 10s     | 1        | ✅ 5/5  | Correcto |
| Test      | 3m 00s    | 3m 30s     | 2        | ⚠️ 4/5  | Necesitó refinamiento |
| Warning   | 2m 45s    | 1m 50s     | 1        | ✅ 5/5  | Eliminó variable correctamente |
| Múltiples | 3m 20s    | 4m 15s     | 1        | ✅ 5/5  | Corrigió todos los errores |
```

---

## 🚨 Casos de Falla Conocidos

### Caso 1: Error en Dependencia Externa
**Síntoma:** Build falla por dependencia no disponible
**Comportamiento esperado:** Copilot no puede resolver (requiere intervención manual)
**Workaround:** Documentar dependencias en README

### Caso 2: Error de Diseño Arquitectural
**Síntoma:** Tests fallan por problema de diseño mayor
**Comportamiento esperado:** Copilot puede sugerir pero no implementar refactoring completo
**Workaround:** Solicitar manualmente cambios específicos

### Caso 3: Conflicto de Merge
**Síntoma:** Branch desactualizado con conflictos
**Comportamiento esperado:** Copilot no puede resolver conflictos automáticamente
**Workaround:** Desarrollador debe resolver conflictos primero

---

## ✅ Checklist de Validación

Antes de dar por validado el sistema, verifica:

- [ ] Copilot responde a todos los escenarios de prueba
- [ ] No se crean comentarios duplicados
- [ ] Los commits de Copilot tienen mensajes descriptivos
- [ ] El CI se vuelve a ejecutar automáticamente después de corrección
- [ ] Los logs del workflow `copilot_autofix_request.yml` son claros
- [ ] No hay errores de permisos
- [ ] El sistema maneja correctamente múltiples PRs simultáneos
- [ ] Las correcciones de Copilot son de alta calidad
- [ ] El sistema no modifica código que no necesita cambios
- [ ] La documentación está actualizada

---

## 📚 Referencias

- [Documentación GitHub Actions](https://docs.github.com/en/actions)
- [GitHub Copilot API](https://docs.github.com/en/copilot)
- [Guía de Uso del Sistema](COPILOT_AGENT_USAGE.md)
- [Ejemplo de Flujo](AUTOFIX_EXAMPLE.md)
