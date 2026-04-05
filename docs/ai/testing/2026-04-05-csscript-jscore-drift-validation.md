# CsScript JavaScriptCore Drift Validation — 2026-04-05

## Tool added
- `scripts/generate_csscript_jscore_drift_audit.py`

## Validation command
```powershell
python scripts/generate_csscript_jscore_drift_audit.py
```

## Expected artifacts
- `docs/ai/implementation/2026-04-05-csscript-jscore-drift-audit.md`
- `docs/ai/implementation/2026-04-05-csscript-jscore-drift-audit.json`

## Observed result
The script executed successfully and generated both artifacts.

## Key outputs confirmed
- counted JavaScriptCore path references in restored `src/script/CMakeLists.txt`
- counted which of those references already exist in the current tree
- counted which are still missing
- grouped the missing references by subtree cluster
- quantified overlap with the current WebKit build's JavaScriptCore references

## Important conclusion
The restored Script baseline already aligns with the majority of the current WebKit-embedded JavaScriptCore tree.
That means the next work should focus on selective adaptation/pruning of the remaining missing clusters rather than questioning the overall recovery direction.
