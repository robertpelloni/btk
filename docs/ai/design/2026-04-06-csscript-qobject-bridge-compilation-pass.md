# CsScript QObject Bridge Compilation Pass

## Context
After the previous engine/value contraction pass, the dominant reduced Stage A frontier moved out of `qscriptengine.cpp` and into `src/script/bridge/qscriptqobject.cpp`.

That shift was important because it meant the recovery effort had progressed from general engine/bootstrap drift into the more BTK/CopperSpice-specific QObject bridge layer that legacy Script and future declarative/BML revival both depend on.

## Goal of this pass
This pass targeted the remaining high-density QObject bridge mismatches that were still preventing restored `CsScript` from completing source compilation under MSVC.

The intent was not to claim runtime-complete Script revival.
The intent was to reduce the compiler frontier from:
- a bridge-layer source failure wall,

into:
- a successful compile-only pass for `CsScript`,
- and then the next truthful frontier, which is now link/dependency materialization in the isolated probe tree.

## Main frontier classes addressed

### 1. Host-call ABI drift in QObject bridge function objects
Restored bridge types such as:
- `QtFunction`
- `QtPropertyFunction`
- `QMetaObjectWrapperObject`

still exposed old-style host-call and host-construct entry points that no longer matched current JSC `NativeFunction` / `NativeConstructor` expectations.

This pass aligns them with the same proxy-entry-point strategy already used successfully in other Script bridge areas:
- add `proxyCall(JSC::ExecState *)`
- add `proxyConstruct(JSC::ExecState *)` where needed
- keep the older business logic in the existing multi-argument helper implementations

This keeps the business logic stable while satisfying the current ABI.

### 2. Enum-style error creation drift
The QObject bridge still used removed enum-style throw sites such as:
- `JSC::TypeError`
- `JSC::GeneralError`
- `JSC::SyntaxError`

This pass continues the now-established design rule for Stage A recovery:
- construct an error object using current helpers
- then pass that object through `throwError(...)`

### 3. Mark/GC root drift
The restored QObject bridge still assumed:
- `Heap::isCellMarked(...)`
- direct `MarkStack::append(JSValue)` / `append(JSObject *)`

Current JSC in this tree instead exposes:
- `Heap::isMarked(...)`
- public `MarkStack::append(...)` only for write-barrier-backed roots

This pass therefore introduces narrow helper glue inside the bridge to root temporary values through public write-barrier forms instead of depending on removed/private mark-stack entry points.

### 4. QString / UString contract drift
The QObject bridge still had many direct conversions that assumed historical implicit compatibility.
This pass continues the explicit-conversion rule already established elsewhere:
- `QScript::convertToString(...)` for `UString -> QString`
- `QScript::toUString(...)` for `QString -> UString`
- `QScript::toIdentifier(...)` for property names/identifiers

### 5. Current `getCallData(...)` / `setPrototype(...)` contracts
The bridge still relied on removed or reshaped signatures such as:
- instance-style `value.getCallData(...)`
- old one-argument `setPrototype(...)`

This pass moves those sites to the current helper/signature forms.

## Why this pass matters
The QObject bridge is the recovered Script layer that most directly mediates between JavaScriptCore and BTK/CopperSpice's QObject/metaobject world.
Moving this layer from compile failure to successful source compilation materially improves confidence that Stage A recovery is no longer blocked on broad, unknown Script breakage.

It also sharpens the next truthful frontier:
- compile-time Script bridge adaptation is now materially deeper,
- the next isolated issue is no longer a large source-level incompatibility cluster,
- it is now the link/dependency frontier in the isolated `probe5` environment.

## Current truthful state after this pass
- `CsScript` source compilation can now succeed in the isolated probe when validated with `MSBuild /t:ClCompile`.
- A full target build in that probe still stops at link because the isolated tree does not currently materialize the required dependent BTK libraries there when project references are disabled.
- This is progress, not parity completion.

## Next recommended direction
The next work should branch in two related directions:
1. keep contracting any newly exposed source mismatches if they appear under broader linked builds,
2. investigate the isolated probe's link/materialization frontier so `CsScript` can move from compile-only success toward true target buildability.