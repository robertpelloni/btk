# BML QtScript Audit Tool Validation — 2026-04-05

## Tool added
- `scripts/generate_bml_qtscript_dependency_audit.py`

## Validation command
```powershell
python scripts/generate_bml_qtscript_dependency_audit.py
```

## Expected artifacts
- `docs/ai/implementation/2026-04-05-bml-qtscript-dependency-audit.md`
- `docs/ai/implementation/2026-04-05-bml-qtscript-dependency-audit.json`

## Observed result
The script executed successfully and generated both artifacts.

## Key outputs confirmed
- quantified QtScript include usage in `src/declarative/`
- quantified `QScript*` identifier footprint
- summarized declarative coupling inside `src/imports/`
- produced a machine-readable JSON artifact and a human-readable markdown summary

## Why this matters
This turns the BML/script gap from a vague impression into a repeatable inventory.
That makes follow-up work on Script restoration or replacement more measurable and easier to review.
