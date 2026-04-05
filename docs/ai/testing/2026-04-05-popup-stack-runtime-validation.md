# BTK Popup Stack Runtime Validation — 2026-04-05

## Goal
Extend the new staged Windows GUI deployment validation path from a single popup/modal scenario into a visible multi-popup scenario.

This pass focuses on verifying that a downstream GUI consumer can:
- stage a runnable Windows BTK GUI app bundle
- show multiple BTK-owned popup windows
- inspect popup stack order and owner-filtered popup stack diagnostics
- validate that popup ownership affects downstream focus-routing decisions for foreign-owner targets

## Sample added
Created:
- `docs/ai/testing/btk-package-popup-stack-runtime-smoke-example/CMakeLists.txt`
- `docs/ai/testing/btk-package-popup-stack-runtime-smoke-example/main.cpp`

## Deployment shape
The sample uses the same app-local deploy pattern introduced in the popup/modal validation pass:

```cmake
find_package(BTK REQUIRED COMPONENTS Core Gui)
include("${BTK_CMAKE_DEPLOY_FILE}")

install(TARGETS btk_package_popup_stack_runtime_smoke RUNTIME DESTINATION .)
btk_copy_library(CsCore .)
btk_copy_library(CsGui .)
btk_copy_plugins(CsGui .)
```

That stages:
- the executable
- `CsCore2.1.dll`
- `CsGui2.1.dll`
- `platforms/CsGuiWin2.1.dll`

This means the sample validates a true staged Windows GUI runtime shape, not merely a DLL-on-PATH developer invocation.

## Runtime scenario under test
The sample constructs:
- popup `popup-a-1` owned by `owner-a`
- popup `popup-a-2` owned by `owner-a`
- foreign target widget owned by `owner-b`
- owner-exclusive focus token owned by `owner-a`

Then it shows both popups and validates that:
- `QApplication::activePopupWidget()` resolves to `popup-a-2`
- `QApplication::btkActivePopupOwnerId()` reports `owner-a`
- `QApplication::btkPopupStackDiagnostics()` returns two entries in the expected order
- `QApplication::btkPopupStackDiagnostics("owner-a")` returns both same-owner popup entries
- `QApplication::btkDescribePopupRelationship(&foreignTarget)` reports `popupAllowed=false`
- `QApplication::btkDescribeFocusDecision(&foreignTarget, Qt::PopupFocusReason)` reports a `Reject` with `blockingOwner=owner-a`

## Validation commands
### Configure
```powershell
cmake -S docs/ai/testing/btk-package-popup-stack-runtime-smoke-example \
      -B build-vs2019/package-popup-stack-runtime-smoke \
      -G "Visual Studio 16 2019" -A x64 \
      -DCMAKE_PREFIX_PATH=C:/Users/hyper/workspace/btk/build-vs2019/install
```

### Build
```powershell
cmake --build build-vs2019/package-popup-stack-runtime-smoke --config Release --parallel 8
```

### Stage
```powershell
cmake --install build-vs2019/package-popup-stack-runtime-smoke \
      --config Release \
      --prefix C:/Users/hyper/workspace/btk/build-vs2019/package-popup-stack-runtime-smoke/stage4
```

### Run
```powershell
C:/Users/hyper/workspace/btk/build-vs2019/package-popup-stack-runtime-smoke/stage4/btk_package_popup_stack_runtime_smoke.exe
```

## Result
The staged popup-stack runtime sample now:
- configures successfully
- builds successfully
- stages successfully
- runs successfully with exit code `0`

## Important finding: stack-presence validation is currently more stable than popup-restoration validation
A more ambitious first attempt was made before settling on the final sample:
- show multiple popups
- close the top popup
- validate restored popup selection and same-owner restoration ordering

That attempted restoration-focused runtime variant reproducibly ended in a Windows access violation during the visible staged GUI scenario.

### Practical interpretation
This suggests an additional runtime hotspot remains in the interaction between:
- nested visible popup closure
- popup stack mutation/restoration
- downstream staged GUI lifecycle timing

The current pass therefore intentionally validates the stable and already-valuable subset:
- popup stack presence
- owner-filtered popup stack reporting
- active popup owner resolution
- popup-aware focus rejection for a foreign owner target

rather than claiming restoration behavior is fully runtime-validated in the visible deployed-app case.

## Why this matters
This pass adds another layer of confidence in BTK's practical downstream story:

### Previously validated
- downstream package discovery
- downstream linking
- downstream visible popup/modal deployment
- BTK-specific owner-aware diagnostics in a staged GUI app

### Newly validated here
- multiple visible popup windows can be staged and observed through downstream BTK popup stack APIs
- owner-filtered popup-stack diagnostics are usable in a real deployed GUI app
- popup ownership directly affects downstream focus-routing diagnostics for foreign-owner targets in that deployed GUI context

## Follow-up recommendations
1. Investigate the access violation seen during runtime popup-restoration validation.
2. Turn that investigation into a focused BTK popup-lifecycle stabilization pass in `src/gui/kernel/qapplication_cs.cpp`.
3. Once stabilized, add a second staged popup-stack sample that validates close/restoration ordering explicitly.
4. Continue using staged GUI runtime samples to catch issues that link-only or hidden-widget tests will never reveal.
