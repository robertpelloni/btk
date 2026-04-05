# BTK Deploy Compatibility Bridge — 2026-04-05

## Context
BTK's package layer intentionally presents BTK-branded names to downstream consumers:
- `find_package(BTK)`
- `BTK::Core`, `BTK::Gui`, etc.
- `BTKDeploy.cmake`

However, the current deploy implementation is still a thin compatibility wrapper around the mature CopperSpice deploy logic:
- `BTKDeploy.cmake` includes `CopperSpiceDeploy.cmake`
- wrapper functions forward to `cs_copy_library(...)` and `cs_copy_plugins(...)`

That is the right compatibility-first architecture, but it creates an important requirement:
- the BTK package must also provide the legacy variable names expected by the underlying CopperSpice deploy script

## Problem observed
During downstream validation of `docs/ai/testing/btk-package-popup-modal-smoke-example/`, the sample used:

```cmake
include("${BTK_CMAKE_DEPLOY_FILE}")
btk_copy_library(CsCore .)
btk_copy_library(CsGui .)
btk_copy_plugins(CsGui .)
```

The deploy step failed because `CopperSpiceDeploy.cmake` expected legacy variables such as:
- `COPPERSPICE_VERSION_API`
- `CS_INSTALL_MODE`
- `CsSignal_Deploy`
- `CsString_Deploy`

but `BTKConfig.cmake` only exported BTK-branded values such as:
- `BTK_VERSION_API`
- `BTK_INSTALL_MODE`
- `BtkSignal_Deploy`
- `BtkString_Deploy`

### Symptom
The GUI plugin deploy helper tried to resolve:
- `CsGuiWin.dll`

instead of the real installed file:
- `CsGuiWin2.1.dll`

That meant the BTK deploy wrapper was discoverable but not fully operational in downstream Windows GUI deployment scenarios.

## Fix
Updated:
- `cmake/BTKConfig.cmake`

BTK now mirrors its BTK-branded package metadata into the legacy deploy variables used by the CopperSpice deploy implementation:
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

## Why this is the correct fix
This preserves the staged-renaming strategy:
- downstream users still consume BTK through BTK-branded package entry points
- internal deploy logic is not force-rewritten prematurely
- mature CopperSpice deployment behavior remains reusable while BTK branding advances externally

In other words, the compatibility bridge fixes behavior at the package boundary rather than destabilizing the deploy subsystem itself.

## Result
After reinstalling the package and re-running downstream validation:
- `include("${BTK_CMAKE_DEPLOY_FILE}")` worked
- `btk_copy_library(...)` copied BTK runtime DLLs into the staged app bundle
- `btk_copy_plugins(CsGui .)` correctly staged `platforms/CsGuiWin2.1.dll`
- the popup/modal downstream GUI example could run as a visible Windows application

## Architectural takeaway
BTK's package layer can continue using a compatibility-first migration pattern successfully, but wrapper APIs must export enough legacy metadata to support the mature substrate underneath them.

This is a recurring migration rule:
- **brand externally, bridge internally**

That rule likely applies to other BTK wrapper layers as the rename proceeds.
