# BML Script Revival Plan — 2026-04-05

## Objective
Define a staged path from today's BML naming/bootstrap layer to a buildable declarative runtime.

## Phase 0 — Truthful stabilization
Status: complete for this pass.

Done:
- BML naming/bootstrap headers exist
- `BTK::Bml` alias path exists when declarative is built
- `WITH_DECLARATIVE` defaults to `OFF`
- configure-time guard now warns and disables declarative when the script-era substrate is missing
- generated dependency inventory now quantifies the scope of missing QtScript/CsScript coupling

## Phase 1 — Script substrate inventory completion
Goal:
- define the minimum Script module surface required for declarative bring-up

Tasks:
1. inventory all QtScript public headers referenced by `src/declarative/`
2. inventory all private bridge headers, especially `qscriptdeclarativeclass_p.h`
3. determine whether any script engine sources already exist elsewhere in history/submodules/upstream references
4. document the expected package/target name (`CsScript`, `BTK::Script`, or compatibility aliases)

Exit criteria:
- clear manifest of required public/private script APIs
- recommendation on restore-vs-replace for the script substrate

## Phase 2 — Minimal Script module restoration
Goal:
- establish enough script infrastructure for `CsDeclarative` to begin compiling

Tasks:
1. create or restore `src/script/` build wiring
2. expose `QtScript` include generation paths expected by `src/declarative/CMakeLists.txt`
3. restore the key public types (`QScriptEngine`, `QScriptValue`, `QScriptContext`, etc.)
4. restore or replace `QScriptDeclarativeClass` bridge support

Exit criteria:
- BTK configure can keep `WITH_DECLARATIVE=ON` without immediately forcing it off
- early declarative compilation can begin, even if not yet complete

## Phase 3 — Declarative core bring-up
Goal:
- compile the main `CsDeclarative` library before chasing optional plugin payloads

Priority order:
1. `src/declarative/qml/`
2. `src/declarative/util/`
3. `src/declarative/debugger/`
4. `src/declarative/graphicsitems/`

Exit criteria:
- `CsDeclarative` builds under MSVC
- staged install exports `QtDeclarative` / BML headers coherently
- downstream `find_package(BTK COMPONENTS Declarative)` becomes possible

## Phase 4 — BML consumer validation
Goal:
- prove the declarative runtime is actually consumable by downstream users

Tasks:
1. add package smoke for `BTK::Bml`
2. validate `BMLEngine`, `BMLComponent`, `BMLContext`, `BMLView`
3. validate BML type registration wrappers
4. document required deployment/runtime considerations

Exit criteria:
- downstream configure, build, and run validation passes

## Phase 5 — Imports/plugin recovery
Goal:
- recover the additional declarative-era plugin payloads in `src/imports/`

Candidate order:
1. `folderlistmodel`
2. `gestures`
3. `shaders`
4. `particles`

Rationale:
- keep the first revival pass focused on core runtime viability before expanding the supported declarative ecosystem

## Recommended mode
Use a **hybrid revival**:
- restore enough script infrastructure to revive the legacy declarative engine
- keep the BML brand as the stable public-facing identity
- modernize behind that identity incrementally instead of attempting an all-new declarative rewrite immediately
