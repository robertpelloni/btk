# CsScript Post-Unblock Diagnostic Capture — 2026-04-05

## Goal
Capture the first unreduced fatal diagnostics after the restored `CsScript` bridge/object pass and JavaScriptCore export-definition unblock moved the direct MSVC build deeper into compilation.

## Process approach
Because the direct `CsScript` build had already started progressing far enough to outlive normal synchronous tool windows, the next diagnostic pass was handled with a detached/background build launch and persisted log capture.

This avoided two problems:
- losing the first later-stage fatal diagnostic to tool output truncation
- repeatedly restarting the build without preserving longer-running progress

## Process preservation
No background processes were terminated.
A fresh process audit was performed with:
- `tasklist`

## Background build launch
A detached MSVC build was launched using WMI process creation so the build could continue independently while its logs were written to disk.

Background command target:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

Captured process:
- `cmd.exe` wrapper PID `120812`

Log files:
- `build-vs2019-script-probe3/csscript-direct-background.out.log`
- `build-vs2019-script-probe3/csscript-direct-background.err.log`

## First post-unblock fatal frontier
The first unreduced fatal diagnostics after the previous linkage/export unblock are now concentrated in:
- `src/script/api/qscriptcontext.cpp`
- `src/script/api/qscriptcontextinfo.cpp`
- `src/script/bridge/qscriptfunction_p.h`

## Representative diagnostics captured
### `qscriptcontext.cpp`
The recovered Script context layer still assumes older call-frame and error APIs such as:
- `ExecState::setException(...)`
- JSC enum-style `ErrorType` / `GeneralError` / `ReferenceError` / `SyntaxError` / `TypeError` / `RangeError` / `URIError`
- `optionalCalleeArguments()` / `setCalleeArguments(...)`
- `ScopeChainNode::copy()`
- direct `WriteBarrier` to raw pointer conversions
- old `RegisterFile::ReturnValueRegister`

Representative errors captured include:
- `error C2039: 'setException': is not a member of 'QTJSC::ExecState'`
- `error C2039: 'ErrorType': is not a member of 'QTJSC'`
- `error C2039: 'optionalCalleeArguments': is not a member of 'QTJSC::ExecState'`
- `error C2039: 'setCalleeArguments': is not a member of 'QTJSC::ExecState'`
- `error C2039: 'copy': is not a member of 'QTJSC::ScopeChainNode'`
- multiple `WriteBarrier<...>` to raw pointer conversion failures
- `error C2039: 'ReturnValueRegister': is not a member of 'QTJSC::RegisterFile'`

### `qscriptcontextinfo.cpp`
The recovered context-info layer still assumes older debugger/codeblock/function metadata contracts such as:
- `CodeBlock::lineNumberForBytecodeOffset(exec, offset)` two-argument shape
- `InternalFunction::info`
- `JSFunction::info`
- `FunctionExecutable::parameterName(...)`

Representative errors captured include:
- `error C2660: 'QTJSC::CodeBlock::lineNumberForBytecodeOffset': function does not take 2 arguments`
- `error C2039: 'info': is not a member of 'QTJSC::InternalFunction'`
- `error C2039: 'info': is not a member of 'QTJSC::JSFunction'`
- `error C2039: 'parameterName': is not a member of 'QTJSC::FunctionExecutable'`

### `qscriptfunction_p.h`
The recovered wrapper layer still includes a no-longer-present header:
- `PrototypeFunction.h`

Representative fatal error captured:
- `fatal error C1083: Cannot open include file: 'PrototypeFunction.h': No such file or directory`

## Interpretation
This is useful because the recovery frontier has now moved again.

It is no longer primarily blocked by:
- restored Script bridge object layout drift
- `JSString.cpp` import/export linkage drift

It is now blocked by a more localized next cluster:
- restored Script context API drift
- restored Script context-info metadata drift
- missing legacy function-wrapper substrate (`PrototypeFunction`-era contract)

## Immediate adaptation reading
### Highest-confidence next fixes
1. `qscriptcontext.cpp`
   - replace `setException(...)` with `globalData().exception = ...`
   - replace old enum-style error dispatch with current `create*Error(...)` + `throwError(...)`
   - adapt `WriteBarrier`/scope-chain usage to `.get()` / `.set(...)`
   - remove or temporarily soften old native-arguments and return-register assumptions

2. `qscriptcontextinfo.cpp`
   - update codeblock line-number calls to the current one-argument form
   - use current `s_info` symbols instead of old `info`
   - replace direct per-parameter-name extraction with a current equivalent

3. `qscriptfunction_p.h/.cpp`
   - determine the current replacement for the historical `PrototypeFunction` dependency
   - likely adapt toward the current `NativeFunctionWrapper` / `InternalFunction` substrate

## Important note
After this diagnostic capture began, local source edits were started against the newly exposed `qscriptcontext*` / function-wrapper frontier while the detached background build continued writing logs from the earlier source snapshot. That is intentional: the log preserves the first reduced post-unblock frontier, while the working tree begins the next adaptation pass without killing the running process.
