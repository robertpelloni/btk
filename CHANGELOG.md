# CHANGELOG

## 0.1.0 - 2026-04-05
- bootstrap project-local version/changelog tracking for ongoing BTK recovery work
- advanced `CsScript` Stage A by adapting current identifier-table handling to `wtfThreadData()`
- replaced obsolete JSC debugger load/unload assumptions with Script-agent routing
- reworked Script source-provider feedback away from inaccessible `UStringSourceProvider` construction
- modernized multiple Script string/value conversion sites around current `UString`, `jsNumber`, `jsString`, and `constructDate` contracts
- contracted major `qscriptdeclarativeclass.cpp` drift and propagated low-risk `QString`/identifier fixes into nearby Script bridge/API files
- revalidated the direct `CsScript` MSVC build probe and documented the newly exposed bridge-header / linkage blocker frontier
