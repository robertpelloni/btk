# BTK Popup/Modal Deploy Validation — 2026-04-05

## Goal
Validate a downstream-installed BTK GUI consumer that exercises both:
- BTK-specific popup/modal ownership diagnostics
- Windows GUI runtime deployment requirements for actual shown windows

This pass intentionally goes beyond link-only or hidden-widget validation. The sample shows real GUI objects, requires the Windows platform plugin, and therefore tests whether downstream BTK packaging is usable for visible GUI applications rather than only for API compilation.

## Sample added
Created:
- `docs/ai/testing/btk-package-popup-modal-smoke-example/CMakeLists.txt`
- `docs/ai/testing/btk-package-popup-modal-smoke-example/main.cpp`

## Scenario under test
The downstream sample constructs:
- a popup widget owned by `owner-popup`
- an application-modal dialog owned by `owner-modal`
- a target widget also owned by `owner-modal`
- an owner-exclusive BTK focus token owned by `owner-popup`

The validation then checks that:
- the popup owner is reported as `owner-popup`
- the modal owner is reported as `owner-modal`
- popup relationship diagnostics report that the popup does **not** allow the target
- BTK focus-decision diagnostics reject the target with `blockingOwner=owner-popup`
- `BTKFocusOverlay` emits non-empty comparison-cluster and mismatch digests for the mixed-owner popup/modal topology

## First runtime finding: installed BTK package was linkable but not yet app-local GUI-runnable
A first direct run of a visible downstream GUI executable failed with the expected Windows platform-plugin error:
- the executable could link against installed BTK libraries
- but a shown `QApplication` GUI path could not start unless a `platforms/` directory containing `CsGuiWin2.1.dll` was available next to the application

This is an important distinction:
- `find_package(BTK)` and linking were already working
- but visible downstream GUI runtime deployment still needed proper plugin layout

In other words, the staged BTK install was adequate as a development package, but not by itself a directly runnable app bundle for visible GUI executables.

## Second finding: BTK deploy wrapper had a compatibility gap
The sample was then upgraded to use the BTK deploy wrapper in downstream CMake:
- `include("${BTK_CMAKE_DEPLOY_FILE}")`
- `btk_copy_library(CsCore .)`
- `btk_copy_library(CsGui .)`
- `btk_copy_plugins(CsGui .)`

This exposed a BTK package compatibility issue:
- `BTKDeploy.cmake` delegates to `CopperSpiceDeploy.cmake`
- the deploy helper expects legacy variables such as `COPPERSPICE_VERSION_API`, `CS_INSTALL_MODE`, and `CsSignal_Deploy`
- `BTKConfig.cmake` previously exported only BTK-branded variables like `BTK_VERSION_API`

Result:
- downstream deploy attempted to copy `CsGuiWin.dll`
- actual installed file was `CsGuiWin2.1.dll`
- plugin deployment failed because the legacy version variable bridge was missing

## Fix implemented
Updated:
- `cmake/BTKConfig.cmake`

The BTK package config now mirrors BTK package metadata into the legacy CopperSpice-shaped variables used by deploy helpers:
- `COPPERSPICE_VERSION_MAJOR`
- `COPPERSPICE_VERSION_MINOR`
- `COPPERSPICE_VERSION_PATCH`
- `COPPERSPICE_VERSION`
- `COPPERSPICE_VERSION_API`
- `CS_INSTALL_MODE`
- `CsLibGuarded_Deploy`
- `CsPointer_Deploy`
- `CsSignal_Deploy`
- `CsString_Deploy`

This is a compatibility-first packaging fix. It preserves BTK branding for downstream consumers while allowing existing deploy helper logic to keep functioning without invasive rewrite.

## Third finding: popup/modal activation order matters for the concrete runtime scenario
The first popup/modal runtime attempt also showed that the test scenario itself was order-sensitive.

Observed behavior in the initial ordering:
- `activeModalOwnerId()` resolved correctly
- `activePopupOwnerId()` remained empty
- popup relationship diagnostics reported `popupOwner=<none>`

The sample was then adjusted so that the runtime establishes the modal dialog first and the popup second inside a timer-driven event-loop sequence:
- `modal.open();`
- `popup.show();`

That ordering produced the desired concrete runtime state for this validation pass.

## Why the timer/event-loop structure was used
Visible popup/modal validation is materially different from the earlier hidden or non-visual package smokes.

This sample uses:
- `QTimer::singleShot(...)`
- `app.exec()`

so that:
- windows actually enter the GUI event loop
- popup/modal bookkeeping becomes observable through BTK runtime APIs
- the application exits cleanly with a pass/fail status code after diagnostics are captured

This avoids hanging while still testing the real GUI lifecycle.

## Final downstream validation flow
Validated successfully with:

### Configure
```powershell
cmake -S docs/ai/testing/btk-package-popup-modal-smoke-example \
      -B build-vs2019/package-popup-modal-smoke \
      -G "Visual Studio 16 2019" -A x64 \
      -DCMAKE_PREFIX_PATH=C:/Users/hyper/workspace/btk/build-vs2019/install
```

### Build
```powershell
cmake --build build-vs2019/package-popup-modal-smoke --config Release --parallel 8
```

### Stage app-local runtime deployment
```powershell
cmake --install build-vs2019/package-popup-modal-smoke \
      --config Release \
      --prefix C:/Users/hyper/workspace/btk/build-vs2019/package-popup-modal-smoke/stage
```

### Run
```powershell
C:/Users/hyper/workspace/btk/build-vs2019/package-popup-modal-smoke/stage/btk_package_popup_modal_smoke.exe
```

## Staged runtime output
The downstream staged application now contains:
- `btk_package_popup_modal_smoke.exe`
- `CsCore2.1.dll`
- `CsGui2.1.dll`
- `platforms/CsGuiWin2.1.dll`

That is the key practical Windows GUI deployment shape required for visible BTK GUI consumers.

## Validation result
The popup/modal sample now:
- configures successfully against installed BTK
- builds successfully downstream
- stages an app-local deployable runtime using BTK deploy wrappers
- runs successfully as a visible GUI executable
- validates BTK popup/modal ownership diagnostics and owner-exclusive rejection semantics

## Meaning for BTK
This pass matters because it closes an important gap between:
- "BTK can be found and linked"
- and
- "a downstream GUI app can actually be staged and run with visible windows on Windows"

It also demonstrates that BTK-specific ownership diagnostics remain available even in the more realistic deployed-app shape.

## Follow-up opportunities
1. Expand the same staged-deploy pattern to other visible GUI runtime examples.
2. Add a richer popup-stack sample that validates multiple popup owners under staged deployment.
3. Consider whether BTK should eventually expose a more BTK-native deployment helper API so downstream users do not need to think in CopperSpice-shaped runtime terms at all.
4. Continue reducing remaining packaging surprises where installed BTK is technically consumable but not yet immediately ergonomic for visible app deployment.
