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

## Recommended next steps
1. Continue building remaining targets incrementally instead of relying only on a single massive full build pass.
2. Prioritize any deferred plugins / optional runtime pieces beyond the now-built core module set.
3. Keep adapting recent BTK additions to actual CopperSpice/BTK APIs rather than Qt-assumed APIs.
4. Expand the downstream smoke application into a slightly richer GUI/package validation example.
5. Improve BTK wrapper ergonomics where practical so downstream examples need fewer CopperSpice-specific adjustments.

## Bottom line
This session materially improved build readiness:
- BTK now configures with MSVC on Windows
- `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, and `CsMultimedia` build successfully
- the project reaches deep into the remaining stack before timing out on heavier full-solution work
- the biggest recent source-compatibility regressions introduced by BTK overlay/diagnostic work were corrected

That is a real step from scaffolding toward usable framework output.
