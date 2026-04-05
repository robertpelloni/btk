# CsScript Declarative/Class Call-ABI Pass

## Context
After the host-function compatibility bridge landed, the next reduced Script recovery frontier moved out of the missing `PrototypeFunction`/`NativeFunctionWrapper` substrate and into restored declarative/class delegate call plumbing.

The newly isolated failures were concentrated in:
- `src/script/bridge/qscriptdeclarativeobject.cpp`
- `src/script/bridge/qscriptclassobject.cpp`

These files still assumed the older QtScript/CopperSpice host-call ABI where delegate call helpers could be assigned directly as:
- `JSValue (ExecState *, JSObject *, JSValue, const ArgList &)`
- `JSObject * (ExecState *, JSObject *, const ArgList &)`

But current JavaScriptCore expects:
- `EncodedJSValue (ExecState *)` for `CallData::native.function`
- `EncodedJSValue (ExecState *)` for `ConstructData::native.function`

## Decision
Adopt the same staged compatibility pattern used in the preceding host-function pass:
- keep the restored delegate business logic mostly intact,
- add small proxy entry points matching the current JSC ABI,
- move the old logic into `*Impl(...)` helpers,
- replace stale enum-style `throwError(exec, JSC::TypeError, ...)` assumptions with current `createTypeError(...)` + `throwError(...)` composition.

## Why this is the right next cut
This pass remains localized and compiler-driven:
1. it preserves historical Script semantics where possible,
2. it avoids speculative repo-wide rewrites,
3. it advances the declarative/BML prerequisite stack incrementally,
4. it keeps the next frontier narrow and auditable.

## Outcome
The direct MSVC probe no longer fails first in the declarative/class delegate call-registration layer.
Instead, the build now progresses materially deeper into the recovered Script + embedded JavaScriptCore compilation flow, reaching later API/JSC compilation work before timing out.

That means the prior reduced frontier has been contracted again.

## Product-level interpretation
This does not mean BTK has reached Qt6/BML/JUCE/GTK/JavaFX/ImGui parity.
It does mean the Script/BML prerequisite stack is moving forward in a truthful, verifiable way, with each pass removing a concrete technical blocker instead of papering over the gap.
