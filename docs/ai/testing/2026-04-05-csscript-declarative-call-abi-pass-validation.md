# CsScript Declarative/Class Call-ABI Pass - Validation

## Objective
Verify that the reduced frontier in restored declarative/class delegate call plumbing is contracted and that the direct `CsScript` MSVC probe progresses deeper.

## Environment
- Host: Windows
- Generator: Visual Studio 16 2019
- Probe tree: `build-vs2019-script-probe5`
- Target: `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- Script mode: `WITH_SCRIPT=ON`

## Safety
A fresh process audit was performed before continuing.
No existing background processes were terminated.

## Validation command
```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe" \
  "C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\src\script\CsScript.vcxproj" \
  /t:Build /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=false
```

## Result
### Previously reduced errors no longer appeared first
The earlier reduced frontier around:
- `qscriptdeclarativeobject.cpp`
- `qscriptclassobject.cpp`

was contracted.

Specifically, the build no longer immediately failed first on:
- assigning old-style delegate call helpers directly to `callData.native.function`
- assigning old-style construct helpers directly to `constructData.native.function`
- `JSC::TypeError` enum-style `throwError(...)` assumptions in that same localized cluster

### New observed behavior
The build advanced materially deeper into later JavaScriptCore/API compilation work and continued compiling until the synchronous validation window timed out.

No new smaller fatal frontier was conclusively isolated from the timed synchronous run itself, which is a positive sign relative to the previous localized hard-stop behavior.

## Interpretation
This pass successfully removed another reduced Script bring-up blocker and pushed the frontier deeper again.
The next best validation step is a detached persisted-log build capture from `probe5` so the first fatal diagnostic beyond this new depth can be recovered without relying on a synchronous timeout window.
