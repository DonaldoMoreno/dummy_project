# 📖 Ejemplo de Flujo de Trabajo Automatizado

Este documento muestra un ejemplo completo de cómo funciona el sistema de revisión y corrección automática de código.

## 🔄 Escenario de Ejemplo

### Paso 1: Desarrollador crea un PR con código problemático

Un desarrollador crea una nueva rama y agrega código con un error:

```cpp
// src/buggy_code.cpp
#include <iostream>

int main() {
    int result = 10 / 0;  // ¡Error! División por cero
    std::cout << "Result: " << result << std::endl;
    return 0
}  // ¡Error! Falta punto y coma
```

```bash
git checkout -b fix/calculation-bug
git add src/buggy_code.cpp
git commit -m "Add calculation feature"
git push origin fix/calculation-bug
```

### Paso 2: Se abre el Pull Request

El desarrollador abre un PR en GitHub. El workflow `build.yml` se ejecuta automáticamente.

**Resultado del CI**: ❌ **FAILED**

```
Error: src/buggy_code.cpp:6:24: error: expected ';' before '}' token
    6 |     return 0
      |             ^
      |             ;
```

### Paso 3: Autofix Workflow se activa

El workflow `copilot_autofix_request.yml` detecta el fallo y publica un comentario:

---

<!-- copilot-autofix:22129117275 -->
@copilot

Los checks de CI fallaron. Por favor:
1) Identifica la **causa raíz** (no solo "workarounds").
2) Corrige el error, agrega/ajusta tests si aplica.
3) Empuja commits **a esta misma rama del PR** para que CI vuelva a correr.

**Workflow run:** https://github.com/DonaldoMoreno/dummy_project/actions/runs/22129117275

**Jobs fallidos (links directos):**
- **build-ubuntu** (failure) — https://github.com/DonaldoMoreno/dummy_project/actions/runs/22129117275/job/60123456789
- **build-windows** (failure) — https://github.com/DonaldoMoreno/dummy_project/actions/runs/22129117275/job/60123456790

**Contexto adicional:**
- No deshabilites checks ni ignores fallas.
- Mantén cambios mínimos y enfocados.

---

### Paso 4: Copilot Analiza y Corrige

Copilot lee los logs de CI, identifica los errores y realiza las correcciones:

```cpp
// src/buggy_code.cpp - CORREGIDO
#include <iostream>

int main() {
    int divisor = 5;  // Usar un divisor válido
    int result = 10 / divisor;
    std::cout << "Result: " << result << std::endl;
    return 0;  // Punto y coma agregado
}
```

Copilot hace commit y push a la misma rama:

```bash
git add src/buggy_code.cpp
git commit -m "Fix: Add missing semicolon and fix division by zero"
git push origin fix/calculation-bug
```

### Paso 5: CI Vuelve a Ejecutarse

El push automático de Copilot dispara el workflow `build.yml` nuevamente.

**Resultado del CI**: ✅ **SUCCESS**

```
✓ build-ubuntu completed successfully
✓ build-windows completed successfully
✓ All checks passed
```

### Paso 6: PR Listo para Merge

El PR ahora muestra:
- ✅ Todos los checks pasados
- 🤖 2 commits: el original + la corrección automática de Copilot
- 👍 Listo para revisión humana y merge

---

## 📊 Métricas del Flujo

| Métrica | Sin Autofix | Con Autofix |
|---------|-------------|-------------|
| Tiempo para identificar error | 15-30 min | < 2 min |
| Tiempo para corregir | 10-20 min | < 2 min |
| Intervención manual | 100% | < 10% |
| Ciclos de CI desperdiciados | 2-3 | 1 |
| Tiempo total | 30-60 min | < 5 min |

---

## 🎯 Tipos de Errores que Copilot Puede Corregir

### ✅ Errores de Sintaxis
- Puntos y coma faltantes
- Paréntesis o llaves sin cerrar
- Comillas mal balanceadas

### ✅ Errores de Compilación
- Includes faltantes
- Tipos de datos incorrectos
- Nombres de funciones mal escritos

### ✅ Warnings de Compilador
- Variables no utilizadas
- Conversiones implícitas peligrosas
- Código inalcanzable

### ✅ Errores Lógicos Comunes
- División por cero
- Desbordamiento de buffer
- Null pointer dereferences

### ✅ Problemas de Estilo
- Formato de código inconsistente
- Convenciones de nombres
- Comentarios faltantes en código complejo

---

## 🔧 Configuración Personalizada

### Modificar Comportamiento del Autofix

Edita `.github/workflows/copilot_autofix_request.yml` para personalizar:

```yaml
# Cambiar el mensaje enviado a Copilot
const body = `${marker}
@copilot

[TU MENSAJE PERSONALIZADO AQUÍ]

**Workflow run:** ${runUrl}
...
`;
```

### Agregar Más Verificaciones

Añade jobs adicionales en `build.yml`:

```yaml
test:
  runs-on: ubuntu-latest
  steps:
    - uses: actions/checkout@v4
    - name: Run Tests
      run: |
        cmake -B build
        cmake --build build
        ctest --test-dir build
```

### Ejecutar Solo en Ciertos Archivos

Modifica los triggers en `build.yml`:

```yaml
on:
  pull_request:
    branches: [ main, develop ]
    paths:
      - 'src/**'
      - 'include/**'
      - 'CMakeLists.txt'
```

---

## 🚨 Limitaciones y Consideraciones

### ⚠️ Casos Donde NO Funciona Automáticamente

1. **Errores de Diseño Complejos**: Arquitectura incorrecta que requiere refactoring mayor
2. **Fallas de Tests con Lógica Incorrecta**: Cuando el código compila pero la lógica es incorrecta
3. **Problemas de Dependencias Externas**: APIs de terceros caídas o versiones incompatibles
4. **Errores de Permisos**: Problemas de acceso a recursos o secretos

### 🔐 Seguridad

- Copilot tiene acceso de **escritura** al PR
- Revisa siempre las correcciones antes de hacer merge
- No confíes ciegamente en cambios automáticos para código crítico de seguridad

### 💡 Mejores Prácticas

1. **Revisa siempre los commits automáticos** antes de aprobar el PR
2. **Usa branches protegidas** con revisiones requeridas
3. **Agrega tests exhaustivos** para validar las correcciones
4. **Mantén logs detallados** en CI para ayudar a Copilot
5. **Documenta casos edge** en los issues del PR

---

## 📚 Recursos Adicionales

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [GitHub Copilot Documentation](https://docs.github.com/en/copilot)
- [CMake Documentation](https://cmake.org/documentation/)
- [Workflow Syntax Reference](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)

---

## 🤝 Soporte

¿Tienes preguntas o problemas?

1. Revisa los [issues existentes](../../issues)
2. Abre un [nuevo issue](../../issues/new) con detalles del problema
3. Incluye logs relevantes de CI
4. Describe el comportamiento esperado vs el actual
