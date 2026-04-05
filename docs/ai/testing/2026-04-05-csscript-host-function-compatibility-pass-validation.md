# CsScript Host-Function Compatibility Pass - Validation

## Objective
Validate that the host-function compatibility pass removes the previous missing-substrate blocker and moves the restored `CsScript` build to a smaller next frontier.

## Environment
- Host: Windows
- Generator: Visual Studio 16 2019
- Probe build tree: `build-vs2019-script-probe5`
- Target: `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- Script mode: `WITH_SCRIPT=ON`

## Process safety
A fresh process audit was performed before the new validation run.
No existing background processes were terminated.
Previously running detached jobs were preserved.

## Validation command
Direct project build via MSBuild:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe" \
  "C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\src\script\CsScript.vcxproj" \
  /t:Build /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=false
```

## Result
### Successful contraction
The build no longer failed first on:
- missing `PrototypeFunction.h`
- missing `NativeFunctionWrapper.h`
- missing `JSGlobalObject::prototypeFunctionStructure()`
- old `qscriptfunction.cpp` proxy signatures

### New reduced fatal cluster
The next observed hard errors moved to legacy declarative bridge plumbing, especially:
- `src/script/bridge/qscriptdeclarativeobject.cpp`

Representative diagnostics captured in the run:
- `error C2440: '=': cannot convert from 'QTJSC::JSValue (__cdecl *)(QTJSC::ExecState *,QTJSC::JSObject *,QTJSC::JSValue,const QTJSC::ArgList &)' to 'QTJSC::NativeFunction'`
- `error C2039: 'TypeError': is not a member of 'QTJSC'`
- `error C2065: 'TypeError': undeclared identifier`

## Interpretation
This validation confirms the host-function compatibility pass was effective:
- the earlier missing-substrate frontier was removed,
- the build advanced deeper into the restored Script/declarative bridge,
- the next work is now more localized and easier to attack.

## Remaining limitation
The validation run still timed out after progressing into later compilation, so this pass is not a full build success.
However, the first fatal errors observed were materially newer and narrower than the prior frontier.

## Recommended next validation step
After adapting the remaining declarative/class-object call sites to the current host-call ABI and replacing old `throwError(..., JSC::TypeError, ...)` assumptions with current helpers, rerun the same direct MSBuild probe to identify the next reduced frontier.
