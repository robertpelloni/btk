# BML Script Manifest Validation — 2026-04-05

## Tool added
- `scripts/generate_bml_script_manifest.py`

## Validation command
```powershell
python scripts/generate_bml_script_manifest.py
```

## Expected artifacts
- `docs/ai/implementation/2026-04-05-bml-script-api-manifest.md`
- `docs/ai/implementation/2026-04-05-bml-script-api-manifest.json`

## Observed result
The script executed successfully and produced both artifacts.

## Key findings confirmed
- only a small subset of declarative public headers directly expose Script coupling
- a much larger private header set depends on Script internals
- the private bridge dependency is concentrated around `qscriptdeclarativeclass_p.h` / `QScriptDeclarativeClass`

## Outcome
The project now has a repeatable manifest separating:
- public Script compatibility requirements
- private declarative bring-up requirements

That distinction makes future `CsScript` restoration planning significantly more concrete.
