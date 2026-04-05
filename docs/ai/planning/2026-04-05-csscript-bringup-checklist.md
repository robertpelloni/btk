# CsScript Bring-Up Checklist for BML Revival — 2026-04-05

## Goal
Turn the new BML Script manifest into a practical first-pass checklist for restoring enough Script infrastructure to begin declarative revival.

## Stage A — Public compatibility checkpoint
Objective:
- make exposed declarative/BML headers source-compatible with an available Script module surface

### Required public Script headers
- `QtScript/QtScript`
- `QtScript/qscriptvalue.h`
- `QtScript/qscriptengine.h`

### Required public types visible through declarative headers
- `QScriptValue`
- `QScriptEngine`
- `QScriptContext`

### Declarative public headers depending on them
- `src/declarative/qml/qdeclarativecomponent.h`
- `src/declarative/qml/qdeclarativeengine.h`
- `src/declarative/qml/qdeclarativecontext.h`
- `src/declarative/qml/qdeclarativeexpression.h`
- `src/declarative/qml/qtdeclarative.h`

### Stage A exit criteria
- public declarative headers are self-consistent again when Script headers are available
- package/install generation can expose a coherent `QtScript` include tree
- BTK can define a future `BTK::Script` / `CsScript` compatibility story without yet compiling `CsDeclarative`

## Stage B — Declarative core compile checkpoint
Objective:
- provide enough Script engine/private bridge surface for `CsDeclarative` compilation to begin

### Core engine/private Script headers
- `QtScript/qscriptclass.h`
- `QtScript/qscriptprogram.h`
- `QtScript/qscriptstring.h`
- `QtScript/qscriptcontext.h`
- `QtScript/qscriptvalueiterator.h`

### Private declarative bridge requirement
- `qscriptdeclarativeclass_p.h`
- `QScriptDeclarativeClass`

### Highest-priority declarative private headers blocked on this layer
- `src/declarative/qml/qdeclarativeengine_p.h`
- `src/declarative/qml/qdeclarativeexpression_p.h`
- `src/declarative/qml/qdeclarativeobjectscriptclass_p.h`
- `src/declarative/qml/qdeclarativecontextscriptclass_p.h`
- `src/declarative/qml/qdeclarativelistscriptclass_p.h`
- `src/declarative/qml/qdeclarativevaluetypescriptclass_p.h`
- `src/declarative/qml/qdeclarativetypenamescriptclass_p.h`
- `src/declarative/qml/qdeclarativepropertycache_p.h`
- `src/declarative/qml/qdeclarativeintegercache_p.h`
- `src/declarative/qml/qdeclarativetypenamecache_p.h`
- `src/declarative/util/qdeclarativelistmodel_p_p.h`

### Stage B exit criteria
- top-level configure no longer needs to force `WITH_DECLARATIVE=OFF` solely because Script is missing
- the `src/declarative/qml/` core can enter real compile iteration under MSVC

## Stage C — Tooling/support checkpoint
Objective:
- restore the additional Script pieces needed by debugger and higher-level helpers

### Additional Script headers/types
- `QtScript/qscriptengineagent.h`
- `QtScript/qscriptcontextinfo.h`
- `QtScript/qscriptclasspropertyiterator.h`

### Declarative areas depending on them
- `src/declarative/debugger/`
- `src/declarative/qml/qdeclarativesqldatabase.cpp`
- `src/declarative/qml/qdeclarativeworkerscript.cpp`
- `src/declarative/util/qdeclarativebind.cpp`
- `src/declarative/util/qdeclarativelistmodel.cpp`

### Stage C exit criteria
- debugger/helper components can be compiled or deliberately deferred with clarity
- declarative utility support is no longer blocked by missing secondary Script APIs

## Implementation order recommendation
1. establish `src/script/` build wiring
2. restore Stage A public headers/types
3. restore Stage B bridge machinery
4. attempt `CsDeclarative` core compilation
5. restore Stage C tooling/support APIs as needed by actual compiler errors

## Why this order is recommended
This keeps BTK aligned with its compatibility-first strategy:
- smallest externally visible compatibility surface first
- largest internal risk surface second
- optional/debug tooling third

It also avoids pretending that debugger or import plugins must be solved before the core Script/declarative bridge exists.
