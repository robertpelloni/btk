# CsScript First Compile Validation — 2026-04-05

## Validation performed
### Experimental Script configure
Validated again that:
```powershell
cmake -S . -B build-vs2019-script-probe3 -G "Visual Studio 16 2019" -A x64 -DWITH_SCRIPT=ON
```

Result:
- configure succeeded
- `Script` remained in the BTK library set
- BTK emitted only an experimental recovery warning, not a forced disable

### Direct Script project build
Invoked MSBuild directly against:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

with project references disabled.

Result:
- build reached direct `CsScript` compilation
- initial parse-collapse issues were reduced by compatibility shim fixes
- deeper JavaScriptCore private API drift errors were surfaced in Script source files

## Important conclusion
This pass achieved the intended milestone:
- move beyond configure viability
- reach the first real compiler-driven `CsScript` diagnostics

The next work is now clearly compiler-guided compatibility adaptation rather than path recovery or baseline selection.
