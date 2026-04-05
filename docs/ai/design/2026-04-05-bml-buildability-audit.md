# BML Buildability Audit — 2026-04-05

## Goal
Determine what prevents BTK's legacy declarative subsystem from being turned into a truly buildable **BML** component.

## Summary
BTK now has a **BML naming/bootstrap layer**, but the underlying declarative runtime is not yet safely buildable.

The core reason is straightforward:
- `src/declarative/` is a legacy `QtDeclarative` / QML1-era subsystem
- it still depends deeply on `QtScript` / `QScript*`
- the current BTK repository does not currently build or expose a corresponding `CsScript` / `QtScript` module

## Findings
### 1. Declarative source tree is substantial and real
`src/declarative/` is not an empty placeholder.
It contains:
- `qml/`
- `graphicsitems/`
- `util/`
- `debugger/`

This means the BML effort has an existing substrate worth studying rather than having to start from zero.

### 2. The declarative subsystem is QML1/QtDeclarative-shaped, not Quick 2 shaped
Representative public classes include:
- `QDeclarativeEngine`
- `QDeclarativeComponent`
- `QDeclarativeContext`
- `QDeclarativeView`

This is a strong sign that the current subsystem is a legacy declarative engine and view stack, not a modern Qt Quick 2 scene graph/runtime.

### 3. The current repo does not expose a Script component at the top level
Top-level BTK optional components currently include things like:
- `Gui`
- `Network`
- `OpenGL`
- `Sql`
- `Svg`
- `WebKit`
- `XmlPatterns`

but there is no corresponding top-level `Script` component in the active build graph.

### 4. `src/declarative/` has hard dependencies on QtScript/QScript APIs
The declarative tree directly references many script-era types and headers, including:
- `QtScript/qscriptengine.h`
- `QtScript/qscriptvalue.h`
- `QtScript/qscriptcontext.h`
- `QScriptEngineAgent`
- `QScriptDeclarativeClass`
- `qscriptdeclarativeclass_p.h`

This is not a soft or incidental dependency. It is structural.

### 5. The installed BTK headers currently do not include a QtScript include tree
Installed headers currently expose:
- `QtCore`
- `QtGui`
- `QtMultimedia`
- `QtNetwork`
- `QtOpenGL`
- `QtSql`
- `QtSvg`
- `QtWebKit`
- `QtXml`
- `QtXmlPatterns`

There is no installed `QtScript/` include directory in the staged install.

## Implication
This means turning on `WITH_DECLARATIVE` today would be misleading unless the missing script-era dependency chain is restored or replaced.

## Correct strategic reading
There are really two different tasks hiding inside the user's request:

### Task A — naming and package identity
This is what the BML bootstrap solved:
- BML headers
- BML type aliases
- `BTK::Bml` alias path
- BML registration wrappers

### Task B — runtime buildability and modernization
This is still open.
It requires one of two major paths:

#### Path 1: Legacy revival
- restore/add a Script module
- restore private script helper infrastructure such as `qscriptdeclarativeclass_p.h`
- get the old declarative engine building again

#### Path 2: Forward modernization
- gradually replace `QScript*`-based internals
- define a future BML runtime architecture
- decide how much legacy `QtDeclarative` code remains worth preserving

## Recommendation
The next safe step is not to blindly enable `WITH_DECLARATIVE`.
The next safe step is to explicitly document and gate the missing dependency chain, which preserves build stability while clarifying the work required for a true BML runtime.
