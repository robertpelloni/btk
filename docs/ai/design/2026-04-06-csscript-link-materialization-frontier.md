# CsScript Link-Materialization Frontier

## Context
After the QObject bridge compilation pass, restored `CsScript` now completes compile-only validation in the isolated `build-vs2019-script-probe5` tree.

That changed the nature of the Stage A frontier:
- the dominant blocker is no longer source adaptation,
- the next truthful frontier is library/dependency materialization for the isolated link step.

## What the isolated link failure actually says
The reduced direct build currently fails at:
- `LINK : fatal error LNK1181: cannot open input file '..\\..\\lib\\Release\\CsCore2.1.lib'`

This means the current isolated `CsScript` project is not primarily blocked by its own translation units anymore.
It is blocked because the import library it explicitly links against is not present in the probe tree at the expected path.

## Project-graph finding
Inspection of `build-vs2019-script-probe5/src/script/CsScript.vcxproj` shows two relevant facts:

1. `CsScript` explicitly links:
- `..\\..\\lib\\Release\\CsCore2.1.lib`

2. `CsScript` also has a `ProjectReference` to:
- `src/core/CsCore.vcxproj`

but uses:
- `<LinkLibraryDependencies>false</LinkLibraryDependencies>`

This combination is important.
It means:
- the Script project is not expecting MSBuild to pass the dependent library automatically as a transitive linked input,
- it is instead expecting the `CsCore` build to materialize its import library at the conventional probe-local path first,
- and then the Script link step consumes that produced file explicitly.

## Why this matters
This frontier is materially narrower and more truthful than the earlier compiler frontiers.
It indicates the next task is not broad JavaScriptCore/API archaeology.
It is one of:
- ensuring the referenced dependent targets are actually built in the probe,
- ensuring they emit the expected import libraries to `build-vs2019-script-probe5/lib/<config>/`,
- or determining whether the current detached/isolated validation strategy needs a different build entry point to materialize those artifacts.

## Current investigative hypothesis
The most likely near-term explanations are:
1. `CsCore` is simply not yet built in the isolated probe before `CsScript` links,
2. `CsCore` is building but has not yet completed within the synchronous validation window,
3. the current detached/synchronous invocation pattern is too narrow to observe the full dependency build and materialization cycle,
4. or another upstream dependency failure in the `CsCore` path is preventing its import library from being emitted.

## Current recommended strategy
Treat this as a build-orchestration/materialization frontier.
Recommended next steps:
1. preserve the successful compile-only milestone,
2. preserve the isolated probe tree and its evidence,
3. use detached persisted-log builds with project references enabled,
4. observe whether `CsCore2.1.lib` appears in `build-vs2019-script-probe5/lib/Release/`,
5. only after that, capture the next truthful linked failure beyond this materialization boundary.