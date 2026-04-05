# BTK Windows Build Enablement — 2026-04-05

## Goal
Move BTK from "compiler unavailable / not locally validated" toward a concretely buildable Windows state.

## Environment findings
A usable MSVC toolchain is available via:
- Visual Studio 2019 Build Tools
- Generator used: `Visual Studio 16 2019`
- Architecture: `x64`

Initial checks showed `cl`, `g++`, and `clang++` were not directly on `PATH`, but CMake was able to discover MSVC through the Visual Studio generator.

## Configure result
Command used:

```powershell
cmake -S . -B build-vs2019 -G "Visual Studio 16 2019" -A x64
```

### Configure outcome
Configure succeeded.

Key findings during configure:
- MSVC C and C++ compilers detected successfully
- Windows SDK selected automatically
- OpenSSL found
- Vulkan SDK not found, so Vulkan support was not configured
- Planned build set included:
  - `Core`
  - `Xml`
  - `Gui`
  - `Multimedia`
  - `Network`
  - `OpenGL`
  - `Sql`
  - `Svg`
  - `WebKit`
  - `XmlPatterns`

## First build failure and fixes
The first concrete build failure occurred in the newly added BTK ownership scaffolding.

### Failure 1: `BtkInputOwner` flags declaration
`BtkInputOwner` originally used `Q_DECLARE_FLAGS(Capabilities, Capability)`.

This did not compile cleanly under the project/toolchain combination used here.

### Fix
Changed:
- `Q_DECLARE_FLAGS(Capabilities, Capability)`

to:
- `using Capabilities = QFlags<Capability>;`

This matches patterns already used elsewhere in the codebase and brought the BTK ownership type into line with established project conventions.

### Related fix
Applied the same style correction to `BtkFocusOverlay`:
- replaced `Q_DECLARE_FLAGS(PanelFlags, Panel)`
- with `using PanelFlags = QFlags<Panel>;`

## Successful target builds
After the flag-declaration fixes and further CopperSpice-compatibility adjustments, the following targets were built successfully in `Release`:

### Built libraries / tools
- `build-vs2019/bin/Release/CsCore2.1.dll`
- `build-vs2019/bin/Release/CsXml2.1.dll`
- `build-vs2019/bin/Release/CsGui2.1.dll`
- `build-vs2019/bin/Release/CsNetwork2.1.dll`
- `build-vs2019/bin/Release/CsOpenGL2.1.dll`
- `build-vs2019/bin/Release/CsSql2.1.dll`
- `build-vs2019/bin/Release/CsSvg2.1.dll`
- `build-vs2019/bin/Release/CsXmlPatterns2.1.dll`
- `build-vs2019/bin/Release/CsMultimedia2.1.dll`
- `build-vs2019/bin/Release/uic.exe`
- `build-vs2019/bin/Release/rcc.exe`

This is a meaningful milestone: BTK is no longer just hypothetically structured for buildability. A substantial Windows runtime slice is now produced successfully, not just isolated core pieces.

## Additional source compatibility fixes required
The recent BTK diagnostic/overlay additions were initially written with more Qt-style APIs than this codebase exposes.

### Adjustments made
To restore compatibility with the actual project string / API surface, the following kinds of changes were required:
- replaced `.arg(...)` with `.formatArg(...)`
- replaced `QFontMetrics::horizontalAdvance(...)` with `QFontMetrics::width(...)`
- replaced `QStringLiteral(...)` with ordinary `QString(...)`
- fixed `QObject::property(...)` / `setProperty(...)` calls to pass `QString` keys instead of raw `const char *`
- corrected `metaObject()->className()` usage to match the actual return type in this project lineage
- corrected newline joins to use `QString("\n")`

### Why this matters
This confirms an important architectural truth about the current BTK effort:
- the project is **not** a drop-in Qt 5/6 source environment
- it retains CopperSpice-era API differences
- any modernization work must be validated against the real substrate rather than assumed from Qt muscle memory

## Full build attempt
A full solution build was attempted with:

```powershell
cmake --build build-vs2019 --config Release --parallel 8
```

## Full build status
The full build advanced substantially:
- core built
- xml built
- gui built
- the build progressed deep into `WebKit`

However, the full build did not complete within the available command timeout window.

### Important nuance
This is no longer the old failure mode of "cannot configure because there is no compiler."

The current state is much better:
- configure works
- major foundational modules build
- the build now reaches very large downstream subsystems
- the remaining issue is build duration / scale, not immediate compiler absence

## Current practical usability status
BTK is now materially more usable on Windows than before this build-enablement pass because:
- the project configures successfully with MSVC
- foundational runtime artifacts exist
- GUI-layer BTK additions now compile cleanly within `CsGui`
- several important downstream modules now build successfully as modular targets (`Network`, `OpenGL`, `Sql`, `Svg`, `XmlPatterns`, `Multimedia`)

That said, the entire framework stack is not yet fully validated end-to-end in this session because the heaviest remaining downstream module (`WebKit`) did not finish in the available timeout window.

## Package install and downstream smoke validation
A local staged install now succeeds to:
- `build-vs2019/install`

Command used:

```powershell
cmake --install build-vs2019 --config Release --prefix C:/Users/hyper/workspace/btk/build-vs2019/install
```

After building `CsWebKit`, the staged install completed successfully and included:
- BTK/CopperSpice CMake package files under `build-vs2019/install/cmake/BTK`
- installed headers
- installed libraries and tools

### Downstream `find_package(BTK)` validation
The existing package smoke example under:
- `docs/ai/testing/btk-package-smoke-example/`

was then validated against the staged install.

Configuration command:

```powershell
cmake -S docs/ai/testing/btk-package-smoke-example \
      -B build-vs2019/package-smoke \
      -G "Visual Studio 16 2019" -A x64 \
      -DBTK_DIR=C:/Users/hyper/workspace/btk/build-vs2019/install/cmake/BTK
```

Build command:

```powershell
cmake --build build-vs2019/package-smoke --config Release --parallel 8
```

Runtime validation:
- `build-vs2019/package-smoke/Release/btk_package_smoke.exe` executed successfully when `build-vs2019/install/bin` was added to `PATH`.

### Downstream sample adjustments required
The smoke sample needed two practical fixes:
- explicitly request `CMAKE_CXX_STANDARD 20`
- use `BtkString::empty()` instead of Qt-style `isEmpty()`

This is important evidence that BTK package consumption now works, but also that the public compatibility layer still reflects real CopperSpice/BTK behavior rather than full Qt-API equivalence.

## Package-path ergonomics improvement
Windows packaging originally installed BTK package files only to:
- `cmake/BTK`

That worked with explicit `BTK_DIR`, but did **not** work when only `CMAKE_PREFIX_PATH=<install-prefix>` was provided.

To improve downstream ergonomics without breaking the existing path, the install rules were expanded so Windows now also installs BTK and CopperSpice package metadata to the more conventional locations:
- `lib/cmake/BTK`
- `lib/cmake/CopperSpice`

This preserves the existing Windows package path while making root-prefix package discovery more realistic for downstream users.

## Expanded downstream package validation
After the packaging-path improvement:
- the core smoke example now configures successfully with `CMAKE_PREFIX_PATH=<install-prefix>` instead of requiring only `BTK_DIR`
- a new GUI-oriented smoke example under `docs/ai/testing/btk-package-gui-smoke-example/` also configures and builds successfully against the staged install
- a new network-oriented smoke example under `docs/ai/testing/btk-package-network-smoke-example/` also configures successfully against the staged install
- new OpenGL- and Svg-oriented smoke examples under `docs/ai/testing/btk-package-opengl-smoke-example/` and `docs/ai/testing/btk-package-svg-smoke-example/` now configure successfully against the staged install
- new Sql- and Multimedia-oriented smoke examples under `docs/ai/testing/btk-package-sql-smoke-example/` and `docs/ai/testing/btk-package-multimedia-smoke-example/` now configure successfully against the staged install
- a richer runtime-oriented GUI smoke example under `docs/ai/testing/btk-package-runtime-smoke-example/` now configures successfully against the staged install
- an integrated runtime smoke example under `docs/ai/testing/btk-package-integrated-runtime-smoke-example/` now configures successfully against the staged install
- a broader platform-smoke example under `docs/ai/testing/btk-package-platform-smoke-example/` now configures successfully against the staged install
- a behavioral runtime smoke example under `docs/ai/testing/btk-package-behavioral-runtime-smoke-example/` now configures successfully against the staged install
- a focus-reason smoke example under `docs/ai/testing/btk-package-focus-reason-smoke-example/` now configures successfully against the staged install
- a popup/modal smoke example under `docs/ai/testing/btk-package-popup-modal-smoke-example/` now configures successfully against the staged install and stages a runnable Windows GUI app bundle via `BTKDeploy`
- a popup-stack runtime smoke example under `docs/ai/testing/btk-package-popup-stack-runtime-smoke-example/` now configures successfully against the staged install and stages a runnable Windows GUI app bundle via `BTKDeploy`
- downstream staged GUI samples can now use `include(BTKDeploy)` directly after `find_package(BTK)` instead of requiring the explicit `${BTK_CMAKE_DEPLOY_FILE}` path

### GUI smoke validation scope
The GUI package smoke path now validates downstream access to:
- `BTK::Gui`
- `<QtGui/BTKFocusOverlay>`
- owner-context / focus-token APIs via `QApplication`
- target-aware overlay diagnostics without relying on an in-repo direct include path

### Network smoke validation scope
The network package smoke path validates downstream access to:
- `BTK::Network`
- `<QtNetwork/QHostAddress>`
- `<QtNetwork/QSslSocket>`
- basic runtime-visible network/SSL API availability through a downstream executable

### OpenGL smoke validation scope
The OpenGL package smoke path validates downstream access to:
- `BTK::OpenGL`
- `BTK::Gui`
- `<QtOpenGL/qgl.h>` / `QGLFormat`
- basic runtime-visible OpenGL format API availability through a downstream executable

### Svg smoke validation scope
The Svg package smoke path validates downstream access to:
- `BTK::Svg`
- `BTK::Gui`
- `<QtSvg/QSvgRenderer>`
- basic SVG parsing/default-size behavior through a downstream executable

### Sql smoke validation scope
The Sql package smoke path validates downstream access to:
- `BTK::Sql`
- `<QtSql/QSqlDatabase>`
- basic downstream database-object API availability through a downstream executable

### Multimedia smoke validation scope
The Multimedia package smoke path validates downstream access to:
- `BTK::Multimedia`
- `<QtMultimedia/QAudioFormat>`
- `<QtMultimedia/QMediaTimeRange>`
- basic multimedia data-structure API availability through a downstream executable

### Runtime-oriented GUI smoke validation scope
The runtime GUI smoke path validates downstream access to:
- `BTK::Gui`
- `<QtGui/BTKFocusOverlay>`
- owner-context and focus-token installation APIs
- target-aware comparison clusters, blocker digests, mismatch digests, and rejection diagnostics for an owner-exclusive scenario

### Integrated runtime smoke validation scope
The integrated runtime smoke path validates downstream access to:
- `BTK::Core`
- `BTK::Gui`
- `BTK::Network`
- `BTK::Svg`
- `BTK` multi-owner overlay diagnostics plus ordinary network/SVG runtime APIs in one downstream executable

### Platform smoke validation scope
The platform smoke path validates downstream access to:
- `BTK::Core`
- `BTK::Gui`
- `BTK::Network`
- `BTK::OpenGL`
- `BTK::Svg`
- `BTK::Sql`
- `BTK::Multimedia`
- an integrated set of representative runtime APIs plus BTK-specific owner-aware overlay diagnostics in one downstream executable

### Behavioral runtime smoke validation scope
The behavioral runtime smoke path validates downstream access to:
- `BTK::Gui`
- owner-exclusive arbitration behavior across same-owner and cross-owner targets
- target decision summaries that distinguish transfer vs reject behavior
- downstream BTK overlay relationship/blocker/mismatch diagnostics for an actual behavioral ownership scenario

### Focus-reason smoke validation scope
The focus-reason smoke path validates downstream access to:
- `BTK::Gui`
- `QApplication::btkDescribeFocusDecision(...)`
- `QApplication::btkWouldBlockFocusChange(...)`
- popup-flavored and active-window-flavored focus reasons in owner-exclusive routing decisions

### Popup/modal deploy smoke validation scope
The popup/modal deploy smoke path validates downstream access to:
- `BTK::Gui`
- `BTKDeploy` compatibility wrappers through direct `include(BTKDeploy)` use after `find_package(BTK)`
- staged Windows GUI deployment using `btk_copy_library(...)` and `btk_copy_plugins(...)`
- popup-owner and modal-owner diagnostics in a visible runtime scenario
- popup/modal mixed-owner mismatch reporting through `BTKFocusOverlay`

### Popup-stack runtime smoke validation scope
The popup-stack runtime smoke path validates downstream access to:
- `BTK::Gui`
- staged Windows GUI deployment using `BTKDeploy`
- multi-popup stack diagnostics in a visible runtime scenario
- owner-filtered popup-stack inspection through `QApplication::btkPopupStackDiagnostics(ownerId)`
- popup-aware focus rejection diagnostics for a foreign-owner target while multiple same-owner popups are active

### Runtime validation
Using `cmd.exe /c` with `build-vs2019/install/bin` added to `PATH`, the downstream console/runtime-oriented samples executed successfully:
- `build-vs2019/package-smoke-prefix/Release/btk_package_smoke.exe`
- `build-vs2019/package-gui-smoke/Release/btk_package_gui_smoke.exe`
- `build-vs2019/package-network-smoke/Release/btk_package_network_smoke.exe`
- `build-vs2019/package-opengl-smoke/Release/btk_package_opengl_smoke.exe`
- `build-vs2019/package-svg-smoke/Release/btk_package_svg_smoke.exe`
- `build-vs2019/package-sql-smoke/Release/btk_package_sql_smoke.exe`
- `build-vs2019/package-multimedia-smoke/Release/btk_package_multimedia_smoke.exe`
- `build-vs2019/package-runtime-smoke/Release/btk_package_runtime_smoke.exe`
- `build-vs2019/package-integrated-runtime-smoke/Release/btk_package_integrated_runtime_smoke.exe`
- `build-vs2019/package-platform-smoke/Release/btk_package_platform_smoke.exe`
- `build-vs2019/package-behavioral-runtime-smoke/Release/btk_package_behavioral_runtime_smoke.exe`
- `build-vs2019/package-focus-reason-smoke/Release/btk_package_focus_reason_smoke.exe`

Using staged app-local deployment produced by `BTKDeploy`, the downstream visible GUI popup/modal and popup-stack samples also executed successfully:
- `build-vs2019/package-popup-modal-smoke/stage/btk_package_popup_modal_smoke.exe`
- `build-vs2019/package-popup-stack-runtime-smoke/stage4/btk_package_popup_stack_runtime_smoke.exe`

## Additional deploy/runtime finding
A meaningful Windows GUI packaging nuance was uncovered during visible popup/modal validation:
- the staged BTK install was already sufficient for `find_package(BTK)` and linking
- but a shown downstream GUI executable still requires a `platforms/` directory containing `CsGuiWin2.1.dll`
- BTK's deploy compatibility wrapper now works for this scenario because `BTKConfig.cmake` mirrors BTK version/install metadata into the legacy `COPPERSPICE_VERSION_*`, `CS_INSTALL_MODE`, and `Cs*_Deploy` variables expected by `CopperSpiceDeploy.cmake`
- `BTKConfig.cmake` now also prepends the BTK package CMake directory to `CMAKE_MODULE_PATH`, allowing natural downstream `include(BTKDeploy)` usage once `find_package(BTK)` has succeeded

A second runtime nuance was then uncovered during the popup-stack pass:
- visible multi-popup stack presence/order diagnostics are working in a staged GUI app
- but an attempted restoration-focused variant which programmatically closed the top popup reproducibly ended in a Windows access violation
- this identifies popup close/restoration in visible deployed-app scenarios as a remaining runtime hotspot rather than a validated behavior

This means BTK is now stronger in two distinct ways:
- installed-package discovery remains BTK-branded
- downstream Windows GUI runtime staging can still reuse mature deploy helper logic without forcing an immediate full rewrite of the deploy subsystem
- staged visible GUI tests are now exposing lifecycle-sensitive popup edge cases that simpler package smokes would miss

## Recommended next steps
1. Continue building remaining targets incrementally instead of relying only on a single massive full build pass.
2. Prioritize any deferred plugins / optional runtime pieces beyond the now-built core module set.
3. Keep adapting recent BTK additions to actual CopperSpice/BTK APIs rather than Qt-assumed APIs.
4. Expand the downstream smoke applications into richer runtime-oriented GUI/package validations.
5. Improve BTK wrapper ergonomics where practical so downstream examples need fewer CopperSpice-specific adjustments.
6. Add further downstream component validation for additional optional/runtime-heavy slices beyond the now-validated `Sql` and `Multimedia` examples.
7. Continue evolving richer runtime package smokes that exercise BTK-specific multi-owner diagnostics rather than only passive type/link validation.
8. Expand integrated downstream runtime validation further so more framework slices are exercised together rather than only component-by-component.
9. Continue growing platform-oriented smoke coverage toward a realistic BTK consumer application skeleton.
10. Continue adding downstream behavioral-runtime scenarios that validate BTK-specific ownership semantics, not just component availability.
11. Continue validating BTKDeploy-based staged GUI app bundles so visible downstream Windows runtime behavior is tested, not only DLL-on-PATH execution.
12. Investigate the access violation seen during visible popup-restoration testing and turn it into a focused popup lifecycle stabilization pass.

## Bottom line
This session materially improved build readiness:
- BTK now configures with MSVC on Windows
- `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, and `CsMultimedia` build successfully
- the project reaches deep into the remaining stack before timing out on heavier full-solution work
- the biggest recent source-compatibility regressions introduced by BTK overlay/diagnostic work were corrected
- downstream BTK deploy wrappers now stage runnable Windows GUI app bundles for both visible popup/modal and visible popup-stack ownership validation scenarios
- visible staged GUI validation is now strong enough to expose a remaining popup close/restoration hotspot that was invisible to lighter-weight smoke tests

That is a real step from scaffolding toward usable framework output.
