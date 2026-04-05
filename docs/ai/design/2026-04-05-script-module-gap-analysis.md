# Script Module Gap Analysis for BML Revival — 2026-04-05

## Purpose
Translate the raw BML/QtScript dependency audit into an architectural reading that can guide BTK's next declarative steps.

## Key quantitative findings
From the generated dependency inventory:
- `src/declarative/` contains **44 files** with QtScript-related includes
- those files reference **19 distinct QtScript-related headers**
- `QScriptValue` appears in **47 files / 822 hits**
- `QScriptEngine` appears in **32 files / 255 hits**
- `QScriptDeclarativeClass` appears in **24 files / 94 hits**
- private bridge headers such as `qscriptdeclarativeclass_p.h` appear repeatedly across the declarative engine
- all **4** `src/imports/` plugin directories show declarative coupling (`folderlistmodel`, `gestures`, `particles`, `shaders`)

## Architectural reading
This is not a narrow missing-header problem.
It is a missing **module family** problem.

The current legacy declarative stack assumes the existence of at least three layers:

### 1. Public script API layer
Examples:
- `QtScript/qscriptvalue.h`
- `QtScript/qscriptengine.h`
- `QtScript/qscriptcontext.h`
- `QtScript/qscriptclass.h`
- `QtScript/qscriptprogram.h`

This is the visible API surface needed for both public declarative headers and core implementation files.

### 2. Private script/declarative bridge layer
Examples:
- `qscriptdeclarativeclass_p.h`
- `private/qscriptdeclarativeclass_p.h`
- `QScriptDeclarativeClass`

This is more serious than simply missing public headers.
It means the old declarative engine relies on internal script-object bridge machinery to expose QML objects, contexts, list models, type namespaces, and expression scopes into the script runtime.

### 3. Tooling/debugging layer
Examples:
- `QScriptEngineAgent`
- `QScriptContextInfo`
- debugger helpers in `src/declarative/debugger/`

This means a script revival limited to just enough API for compile success may still leave debugger and inspection surfaces incomplete.

## Dependency concentration
### Highest-pressure zone: `src/declarative/qml`
The `qml/` subtree dominates the script include count.
This is expected because it contains:
- engine internals
- expression evaluation
- context/object/type script classes
- import/include helpers
- SQL/XMLHttpRequest worker integration

This strongly suggests that BML revival cannot be solved only in `util/` or only through public wrappers.
The engine core itself is script-shaped.

### Secondary zone: `src/declarative/util`
`util/` contains script-sensitive models and helpers such as:
- list models
- binding helpers
- worker-agent helpers

These are not peripheral demos; they are user-facing declarative utilities.

### Additional zone: `src/imports`
Even though `src/imports/` is not currently wired by a top-level `CMakeLists.txt`, it matters strategically.
It contains plugin payloads that assume a functioning declarative runtime:
- `folderlistmodel`
- `gestures`
- `particles`
- `shaders`

So a future “BML works” claim should eventually consider import/plugin revival too, not only `CsDeclarative` library compilation.

## Implications for BTK strategy
### What BTK should not do next
BTK should not simply flip `WITH_DECLARATIVE` on and hope to patch isolated compile errors.
The audit shows the missing dependency chain is too broad for that approach to be efficient or honest.

### What BTK can safely do next
BTK can proceed through a staged hybrid plan:
1. restore or recreate a minimal Script module boundary
2. restore the specific script/declarative private bridge primitives needed by the engine core
3. bring up `CsDeclarative` itself before chasing import plugins
4. only then extend toward `src/imports/` and richer BML-facing examples

## Recommended architectural stance
Treat the missing script layer as a **foundational prerequisite subsystem**, not a declarative bug.

That framing matters because it changes the work from:
- “fix a couple of broken declarative files”

to:
- “reintroduce or replace the runtime substrate the old declarative engine was designed around”

That is the right level of seriousness for the BML revival effort.
