# BML Revival Options — 2026-04-05

## Option 1 — Legacy declarative revival
Goal:
- make the existing `src/declarative/` subsystem build again largely as-is

Needed work:
- restore/add Script module support
- restore `QtScript` public/private headers
- restore `QScriptDeclarativeClass` support infrastructure
- validate `CsDeclarative` under MSVC

Pros:
- fastest path to a running declarative stack
- reuses substantial existing code

Cons:
- revives a legacy QML1/QtDeclarative architecture
- may increase technical debt before modernization

## Option 2 — Hybrid revival
Goal:
- restore just enough script/declarative infrastructure to get BML bootstrapped
- then progressively modernize internals behind the BML branding layer

Pros:
- pragmatic middle ground
- aligns with BTK's compatibility-first pattern

Cons:
- requires careful staged migration discipline

## Option 3 — Forward replacement
Goal:
- treat BML as a fresh declarative runtime effort
- use the existing declarative tree mainly as a reference source

Pros:
- clean architecture opportunity
- can target modern BTK semantics directly

Cons:
- much larger implementation effort
- delays time-to-first-buildable declarative runtime

## Current recommendation
Prefer **Option 2**.
It matches the rest of the BTK migration strategy:
- preserve useful substrate
- add branding and package compatibility first
- modernize in controlled stages
