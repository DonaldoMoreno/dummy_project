# 📋 Referencia Rápida: Copilot Autofix

## ⚡ Respuesta Rápida

**P: ¿Cómo llamo al agente Copilot desde un GitHub Action para solucionar errores en un PR?**

**R:** El agente Copilot se invoca automáticamente cuando tu workflow de CI falla. Necesitas:

1. Un workflow de CI (ej: `build.yml`)
2. Un workflow que detecte fallos e invoque a Copilot (ej: `copilot_autofix_request.yml`)

```yaml
# .github/workflows/copilot_autofix_request.yml
name: Copilot Autofix Request
on:
  workflow_run:
    workflows: ["Build"]  # Tu workflow de CI
    types: [completed]

permissions:
  actions: read
  pull-requests: write
  issues: write

jobs:
  request-copilot-fix:
    runs-on: ubuntu-latest
    if: ${{ github.event.workflow_run.conclusion == 'failure' }}
    steps:
      - uses: actions/github-script@v7
        with:
          script: |
            // Publicar comentario con @copilot
            await github.rest.issues.createComment({
              owner: context.repo.owner,
              repo: context.repo.repo,
              issue_number: PR_NUMBER,
              body: '@copilot Fix the CI errors'
            });
```

---

## 🎯 Comandos Esenciales

### Verificar Estado del Workflow
```bash
# Ver workflows recientes
gh run list --limit 5

# Ver detalles de un run
gh run view RUN_ID

# Ver logs de un job fallido
gh run view RUN_ID --log-failed
```

### Crear PR de Prueba
```bash
git checkout -b test/copilot-fix
# Introduce un error intencional
git commit -am "Test: Trigger Copilot autofix"
git push origin test/copilot-fix
gh pr create --title "Test Copilot Autofix"
```

### Verificar Invocación de Copilot
```bash
# Ver comentarios en el PR
gh pr view PR_NUMBER --comments

# Ver commits recientes (incluye commits de Copilot)
git log --oneline -5
```

---

## 📐 Anatomía del Sistema

```
┌─────────────────────────────────────────────────────────┐
│  1. Developer pushes code to PR                         │
└──────────────────┬──────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────┐
│  2. Build Workflow (.github/workflows/build.yml)       │
│     - Compiles code                                     │
│     - Runs tests                                        │
│     - Exits with failure code if errors                 │
└──────────────────┬──────────────────────────────────────┘
                   │ (on failure)
                   ▼
┌─────────────────────────────────────────────────────────┐
│  3. Copilot Autofix Request Workflow                   │
│     (.github/workflows/copilot_autofix_request.yml)    │
│     - Triggered by workflow_run event                   │
│     - Fetches failed jobs and logs                      │
│     - Posts comment mentioning @copilot                 │
└──────────────────┬──────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────┐
│  4. Copilot Agent Analyzes & Fixes                     │
│     - Reads PR context and error logs                   │
│     - Identifies root cause                             │
│     - Implements fix                                    │
│     - Commits and pushes to PR branch                   │
└──────────────────┬──────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────┐
│  5. Build Workflow Re-runs                             │
│     - Triggered by Copilot's push                       │
│     - If passes: PR is ready                            │
│     - If fails: Can retry with Copilot again            │
└─────────────────────────────────────────────────────────┘
```

---

## 🔑 Configuración Mínima

### Archivo 1: `build.yml`
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: make build
      - run: make test
```

### Archivo 2: `copilot_autofix_request.yml`
```yaml
name: Copilot Autofix Request
on:
  workflow_run:
    workflows: ["Build"]
    types: [completed]

permissions:
  actions: read
  pull-requests: write
  issues: write

jobs:
  request-copilot-fix:
    runs-on: ubuntu-latest
    if: ${{ github.event.workflow_run.conclusion == 'failure' }}
    steps:
      - uses: actions/github-script@v7
        with:
          script: |
            const prs = context.payload.workflow_run.pull_requests;
            for (const pr of prs) {
              await github.rest.issues.createComment({
                owner: context.repo.owner,
                repo: context.repo.repo,
                issue_number: pr.number,
                body: '@copilot CI failed. Please fix.'
              });
            }
```

---

## 🛠️ Personalización Común

### Solo invocar en branches específicos
```yaml
if: |
  github.event.workflow_run.conclusion == 'failure' &&
  startsWith(github.event.workflow_run.head_branch, 'feature/')
```

### Incluir enlaces a logs específicos
```javascript
const jobs = await github.rest.actions.listJobsForWorkflowRun({
  owner, repo, run_id: runId
});
const failedJobs = jobs.data.jobs
  .filter(j => j.conclusion === 'failure')
  .map(j => `- [${j.name}](${j.html_url})`);
```

### Agregar contexto del PR
```javascript
const prDetails = await github.rest.pulls.get({
  owner, repo, pull_number: pr.number
});
const context = `
Files changed: ${prDetails.data.changed_files}
Lines: +${prDetails.data.additions} -${prDetails.data.deletions}
`;
```

---

## ⚠️ Troubleshooting

| Problema | Causa | Solución |
|----------|-------|----------|
| Workflow no se dispara | Nombre del workflow no coincide | Verificar `workflows: ["Build"]` |
| Error de permisos | Permisos insuficientes | Agregar `pull-requests: write` |
| Copilot no responde | No está mencionado correctamente | Usar `@copilot` no `@github/copilot` |
| Comentarios duplicados | No hay protección | Ver código de ejemplo con marker |
| Workflow no encuentra PR | Push directo a main | Solo funciona con PRs |

---

## 📊 Métricas Típicas

| Métrica | Valor Típico |
|---------|--------------|
| Tiempo de detección | 1-3 minutos |
| Tiempo de análisis de Copilot | 2-5 minutos |
| Tiempo de corrección | 1-3 minutos |
| Tasa de éxito (primer intento) | 70-85% |
| Total end-to-end | 5-10 minutos |

---

## ✅ Checklist de Implementación

- [ ] `build.yml` existe y funciona correctamente
- [ ] `copilot_autofix_request.yml` está configurado
- [ ] Nombres de workflows coinciden exactamente
- [ ] Permisos están configurados correctamente
- [ ] GitHub Copilot está habilitado en el repositorio
- [ ] Comentario menciona `@copilot` correctamente
- [ ] Hay protección contra comentarios duplicados
- [ ] Logs del workflow son claros y útiles
- [ ] Se probó con un PR de prueba
- [ ] La documentación está actualizada

---

## 📚 Documentación Completa

- **[Guía de Uso Completa](COPILOT_AGENT_USAGE.md)** - Tutorial detallado paso a paso
- **[Ejemplo de Flujo](AUTOFIX_EXAMPLE.md)** - Demostración real del sistema
- **[Escenarios de Prueba](TEST_SCENARIOS.md)** - Casos de prueba para validar
- **[README Principal](../README.md)** - Visión general del proyecto

---

## 🆘 Obtener Ayuda

Si tienes problemas:

1. **Verifica los logs del workflow:**
   ```bash
   gh run view --log
   ```

2. **Revisa el comentario generado:**
   ```bash
   gh pr view PR_NUMBER --comments
   ```

3. **Comprueba permisos:**
   - Settings → Actions → General → Workflow permissions
   - Debe estar en "Read and write permissions"

4. **Abre un issue** con:
   - Logs del workflow
   - Configuración actual
   - Comportamiento esperado vs actual

---

## 🎓 Ejemplos de Uso

### Ejemplo 1: Error de Sintaxis
```
❌ Build falla: "expected ';' before '}'"
✅ Copilot agrega el punto y coma faltante
```

### Ejemplo 2: Include Faltante
```
❌ Build falla: "'cout' is not a member of 'std'"
✅ Copilot agrega #include <iostream>
```

### Ejemplo 3: Test Fallido
```
❌ Test falla: "Expected 5, got 3"
✅ Copilot corrige la lógica de la función
```

---

**💡 Tip:** Mantén esta referencia abierta mientras implementas el sistema por primera vez.
