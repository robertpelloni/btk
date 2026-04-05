# CsScript Private API Adaptation Matrix — 2026-04-05

## Goal
Turn the first direct `CsScript` compiler failures into a structured adaptation matrix.

## Matrix
| Historical contract / symbol | First hotspot | Current-tree status | Likely adaptation reading | Suggested next action |
|---|---|---|---|---|
| `JSC::setCurrentIdentifierTable(...)` | `qscriptengine_p.h` | Current equivalent exists through `wtfThreadData().setCurrentIdentifierTable(...)` in current JavaScriptCore | API moved from old direct helper into thread-data access path | Patch `qscriptengine_p.h` to use current thread-data API |
| `scriptLoad` / `scriptUnload` on debugger | `qscriptengine_p.h` | Not present with old contract | Historical debugger feedback integration likely drifted or became obsolete | Mark as compatibility-gap candidate; inspect current debugger hooks before patching |
| `UString::data()` / `size()` / `rep()` / `from(...)` | `qscriptengine_p.h`, `qscriptdeclarativeclass.cpp`, `qscriptdeclarativeobject.cpp` | Contract drift evident; compile shows old member access no longer valid | String/value conversion layer changed materially | Build a focused `UString` conversion adaptation pass |
| `jsNumber(exec, value)` / `jsString(exec, value)` old signatures | `qscriptengine_p.h`, `qscriptdeclarativeclass.cpp` | Current helpers exist but signatures differ | Primitive creation helpers changed | Patch call sites to current helper signatures |
| `JSVariableObjectData` | `qscriptactivationobject_p.h`, `qscriptstaticscopeobject_p.h` | Old layering not present in same form | Variable-object internals changed substantially | Study current `JSVariableObject` / symbol-table data model before patching |
| `OverridesMarkChildren` flag usage | `qscriptstaticscopeobject_p.h`, `qscriptobject_p.h`, `qscriptqobject_p.h` | Old flag contract not present | GC/marking contract drift | Treat as structure/GC API modernization task |
| `Structure::create(...)` old arguments | multiple bridge headers | Current function exists but signature/layout differs | Structure creation API drift | Patch only after inspecting current expected constructor shape |
| `symbolTablePut...(...)` helpers | activation/static scope objects | Current symbol-table helpers differ | Symbol-table mutation APIs changed | Build a dedicated symbol-table adaptation subtask |
| `MarkStack` under runtime assumptions | already partially fixed | Current equivalent exists in `heap/MarkStack*` | Location drift, mostly solved at path level | Keep current remap; remaining compile issues now semantic |
| `StringBuilder.h` under runtime assumptions | already partially fixed | Current equivalent exists in `wtf/text/StringBuilder.h` | Location drift, mostly solved at path level | Keep current remap |
| legacy `Regex*` yarr / `pcre` / `wrec` source expectations | `src/script/CMakeLists.txt` | Removed from Stage A source list | Historical engine contract no longer matches current source layout | Continue treating as pruned/contracted unless future source errors prove otherwise |

## Highest-priority next study
### 1. `qscriptengine_p.h`
This file appears to be the central compatibility hub.
It should be treated as the first adaptation target because it concentrates many of the earliest and most repeated private-API mismatches.

### 2. Variable-object / symbol-table bridge files
Next after `qscriptengine_p.h`:
- `qscriptactivationobject_p.h/.cpp`
- `qscriptstaticscopeobject_p.h/.cpp`

These will likely require more semantic adaptation than simple renames.

### 3. Declarative bridge files
Then:
- `qscriptdeclarativeclass.cpp`
- `qscriptdeclarativeobject.cpp`

These are especially important for future BML bring-up, but should probably follow the more central engine/symbol-table fixes first.

## Current recommendation
Proceed in this order:
1. patch obvious current-equivalent cases in `qscriptengine_p.h`
2. then inspect and adapt symbol-table / variable-object internals
3. then revisit declarative bridge code once core private engine glue is less volatile
