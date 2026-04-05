# CsScript Stage A Drift Contraction Validation — 2026-04-05

## Validation performed
### Drift audit regeneration
Command:
```powershell
python scripts/generate_csscript_jscore_drift_audit.py
```

Result:
- audit regenerated successfully
- restored Script CMake JavaScriptCore references now report **0 missing** paths in the current tree

### Experimental Script configure probe
Command:
```powershell
cmake -S . -B build-vs2019-script-probe3 -G "Visual Studio 16 2019" -A x64 -DWITH_SCRIPT=ON
```

Result:
- configure succeeded
- BTK emitted an experimental Script recovery warning
- `Script` remained enabled in the configured BTK library set

Observed configured BTK libraries now included:
- `Core Xml Gui Script Multimedia Network OpenGL Sql Svg WebKit XmlPatterns`

### Build attempt
Command:
```powershell
cmake --build build-vs2019-script-probe3 --config Release --target CsScript
```

Observed result:
- build began successfully
- MSBuild spent the available timeout rebuilding prerequisite targets such as `CsCore`
- the run timed out before reaching a first direct `CsScript` compile-failure diagnosis

## Conclusion
The contraction pass achieved its immediate Stage A objective:
- the recovered `CsScript` build file no longer references missing JavaScriptCore paths
- BTK can now configure with `WITH_SCRIPT=ON` experimentally

The next step is to continue from configure-time viability into actual compiler-driven diagnosis of remaining Script build issues.
