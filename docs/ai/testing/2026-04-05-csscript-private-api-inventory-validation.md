# CsScript Private API Inventory Validation — 2026-04-05

## Validation performed
### Direct Script build reached source-level diagnostics
Validated through direct MSBuild invocation against:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

This confirmed that the current Stage A effort is far enough along to surface true source-level compatibility issues rather than only path/configuration issues.

### Current-tree equivalent search performed
Validated representative current-tree evidence for:
- moved identifier-table handling through `wtfThreadData().setCurrentIdentifierTable(...)`
- moved `MarkStack` location under `heap/`
- moved `StringBuilder` location under `wtf/text/`
- current yarr naming using `Yarr*` rather than older `Regex*` patterns

## Conclusion
The private-API adaptation inventory is grounded in:
- direct compiler diagnostics
- current embedded JavaScriptCore source inspection

This is sufficient to begin a targeted compatibility patch pass starting with `qscriptengine_p.h`.
