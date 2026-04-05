# BML Script Manifest Analysis — 2026-04-05

## Purpose
Interpret the generated BML Script API manifest in terms of what BTK must actually restore before `WITH_DECLARATIVE` can ever become a real build path.

## Core result
The new manifest splits the problem into two very different obligations:

### 1. Public Script compatibility obligation
The generated manifest shows only **5 declarative public headers** with direct Script coupling:
- `qml/qdeclarativecomponent.h`
- `qml/qdeclarativeengine.h`
- `qml/qdeclarativecontext.h`
- `qml/qdeclarativeexpression.h`
- `qml/qtdeclarative.h`

This is important because it means the *downstream* BML-facing Script surface is smaller than the full internal dependency graph.

The public-facing pressure is concentrated around:
- `QScriptValue`
- `QScriptEngine`
- `QScriptContext`
- aggregate `QtScript/QtScript`

This suggests BTK could define a **public-compatibility checkpoint** for Script restoration.
That checkpoint is smaller than full declarative revival.

### 2. Private declarative-bridge obligation
The generated manifest shows **25 private declarative headers** with Script coupling.
More importantly, several of them are not using only ordinary public Script APIs. They depend on:
- `qscriptdeclarativeclass_p.h`
- `QScriptDeclarativeClass`

These appear in the classes that bridge the declarative engine to the script runtime:
- object script classes
- context script classes
- list script classes
- value type script classes
- typename cache / property cache layers
- list model helpers

This means BTK cannot restore BML merely by shipping public Script headers.
It must also restore or replace the **private runtime bridge semantics** that old QtDeclarative expected.

## Why this is strategically useful
Before this manifest, the BML/Script blocker could be described only in broad terms.
Now BTK has an explicit staged reading:

### Checkpoint A — public API compatibility
Objective:
- make the public `QtDeclarative` / BML headers self-consistent again from a source-compatibility perspective

Likely minimum:
- `QtScript/qscriptvalue.h`
- `QtScript/qscriptengine.h`
- forward declarations / definitions for `QScriptValue`, `QScriptEngine`, `QScriptContext`
- aggregate `QtScript/QtScript`

### Checkpoint B — declarative compile substrate
Objective:
- satisfy enough private Script infrastructure for `CsDeclarative` to compile

Likely minimum:
- `qscriptdeclarativeclass_p.h`
- `QScriptDeclarativeClass`
- `QScriptClass`
- `QScriptProgram`
- private interaction points used by:
  - `qdeclarativeengine_p.h`
  - `qdeclarativeobjectscriptclass_p.h`
  - `qdeclarativecontextscriptclass_p.h`
  - `qdeclarativelistscriptclass_p.h`
  - `qdeclarativevaluetypescriptclass_p.h`
  - `qdeclarativetypenamescriptclass_p.h`
  - `util/qdeclarativelistmodel_p_p.h`

## Architectural interpretation
The manifest makes the recommended BTK path even clearer:

- **do not** attempt full declarative build enablement first
- **do** treat Script restoration as a staged prerequisite program
- **do** target a public-compatibility checkpoint before a full declarative compilation checkpoint
- **do** keep BML branded externally while solving the internal legacy substrate in controlled layers

## Recommended next implementation step
Create a **Script module bring-up checklist** organized by concrete files/types:
1. public headers required by exposed declarative headers
2. private bridge headers required by declarative internals
3. optional debugger/tooling headers deferred until after core bring-up
4. explicit first-pass compile target: get `src/declarative/qml/` engine core past initial configure/build barriers

That is now specific enough to drive real implementation work instead of only analysis.
