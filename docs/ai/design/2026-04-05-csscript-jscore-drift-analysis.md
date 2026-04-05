# CsScript JavaScriptCore Drift Analysis — 2026-04-05

## Purpose
Interpret the restored Script baseline against the current WebKit-embedded JavaScriptCore snapshot so BTK can focus Stage A on the real adaptation work.

## Key quantitative result
The generated drift audit shows that the restored historical Script baseline is **closer to viable than the raw warning initially suggested**.

Headline counts:
- `src/script/CMakeLists.txt` references **464** JavaScriptCore paths
- **408** of those references already exist in the current tree
- only **56** are currently missing
- **147** existing Script references also appear directly in current `src/webkit/CMakeLists.txt`

## Strategic reading
This is a very strong signal that the Stage A recovery effort is not fighting a completely alien third-party substrate.
Most of the historical Script module's JavaScriptCore expectations still line up with the current embedded tree.

That means the recovery problem is now:
- **narrow and clusterable**

not:
- broad or foundationally impossible

## Main missing clusters
The missing references are concentrated in a few clear groups:
- `runtime` — 20
- `pcre` — 10
- `wrec` — 9
- `yarr` — 8
- `generated` — 3
- `wtf` — 3
- `jit` — 2
- `unicode` — 1

This is useful because it suggests a cluster-by-cluster adaptation strategy rather than random patching.

## Most important interpretation by cluster
### 1. `pcre`
The restored historical Script baseline expects a dedicated legacy `pcre/` subtree inside JavaScriptCore.
The current WebKit JavaScriptCore snapshot does not expose that subtree in the same way.

This likely means Script recovery will need one of:
- source-list pruning
- substitution with current regex/yarr facilities
- compatibility-layer include/source redirection

### 2. `wrec`
The historical baseline expects old `wrec` artifacts that are absent now.
This strongly suggests an old JIT/regex/codegen subsystem drift rather than a BTK-local error.

### 3. `runtime`
This is the most important cluster because it includes files like:
- `Collector.cpp`
- `MarkStack.cpp`
- `JSImmediate.cpp`
- `PrototypeFunction.cpp`
- `GlobalEvalFunction.cpp`

These names suggest meaningful internal architecture drift inside JavaScriptCore across eras.
This is likely where the real adaptation work will become semantically non-trivial.

### 4. `yarr`
The historical Script baseline expects older `Regex*`-named yarr artifacts while the current WebKit tree exposes a different yarr shape.
That again points to upstream engine evolution rather than a missing BTK checkout problem.

## Positive signal from overlap with current WebKit build
The fact that **147** restored Script references already appear in current `src/webkit/CMakeLists.txt` is especially encouraging.

It means BTK is not just pointing at files that happen to exist; a substantial subset is already accepted by the current project build graph in another subsystem.

That strengthens confidence that the restored Script baseline can be adapted rather than discarded.

## Recommended next technical move
Stage A should now proceed as a **clustered JavaScriptCore contract reduction pass**:
1. separate references into:
   - already valid and keepable
   - obsolete and removable
   - obsolete but replaceable with current equivalents
2. start with the heaviest missing clusters:
   - `runtime`
   - `pcre`
   - `wrec`
3. avoid touching `CsScriptTools` until `CsScript` itself reaches first compile viability

## Bottom line
The restored Script baseline is materially viable as a starting point.
The new audit shows BTK already has the majority of the expected JavaScriptCore surface in-tree.
So the recovery effort should now focus on **selective adaptation and pruning**, not on questioning whether Stage A recovery is the right path.
