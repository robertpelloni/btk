# HANDOFF

## Completed
- Added `external/juce`, `external/ultimatepp`, and `external/bobui-reference` as git submodules.
- Rebranded the root CMake project/package metadata to BTK.
- Added `cmake/BTKConfig.cmake`, `cmake/BTKConfigVersion.cmake`, `cmake/BTKMacros.cmake`, and `cmake/BTKDeploy.cmake`.
- Added a first BML bootstrap layer over the legacy declarative subsystem, including BML alias headers, `BTK::Bml` alias scaffolding, and a default-off `Declarative` build gate.
- Added a BML buildability audit and a configure-time declarative guard which warns and forces `WITH_DECLARATIVE=OFF` if the legacy declarative runtime is requested without its missing QtScript/CsScript-era dependencies.
- Added a generated QtScript dependency inventory for BML revival (`scripts/generate_bml_qtscript_dependency_audit.py`) plus generated markdown/JSON artifacts quantifying the declarative script footprint and `src/imports/` declarative coupling.
- Added a generated BML Script API manifest (`scripts/generate_bml_script_manifest.py`) which separates public Script-facing declarative headers from private declarative bridge dependencies and identifies the smallest obvious public-vs-private Script restoration checkpoints.
- Added BTK package target aliases in both `BTK::Btk<Component>` and `BTK::<Component>` forms.
- Added a first wave of public C++ BTK compatibility headers: `BTKCore`, `BTKString`, `BTKPointer`, and `BTKSignal`.
- Added first-wave multi-user ownership/focus scaffolding: `BtkInputOwner`, `BtkFocusToken`, `BtkInputArbitrator`, `BtkInputRouteRequest`, and `BtkInputRouteResult`.
- Added the first narrow real GUI integration in `QApplicationPrivate::setFocusWidget(...)` via BTK focus-token arbitration and widget owner/surface context helpers.
- Extended BTK ownership integration into popup/modal gating through `tryModalHelper(...)` and `isWindowBlocked(...)`.
- Added owner-aware diagnostic/reporting helpers on `QApplication` for focus tokens, widget owner context, popup/modal owners, and routing decisions.
- Extended popup stack behavior so restoration now prefers same-owner popups and avoids forcing first-popup focus-out on unrelated owners by default.
- Added `BTKFocusDiagnostics` as a QtGui-level diagnostics adapter for future developer overlays and tooling.
- Added `BTKFocusOverlay` as the first visual BTK focus/owner developer overlay scaffold.
- Refined `BTKFocusOverlay` into a richer HUD pass with summary chips, structured sections, owner-group summaries, blocker-group summaries, relationship digest summaries, popup-stack summaries, token summaries, blocked-route summaries, target-widget decision context, panel-based visibility control, preset-oriented panel modes, and blocked-only filtering.
- Refined `BTKFocusOverlay` further with a dedicated relationship panel, relationship-count summary chip, comparison-cluster summaries, target-owner comparison clusters, blocked-reason grouping, blocker drilldown grouping, mismatch highlighting, count-prioritized owner/blocker ordering, relationship digest extraction in `BtkFocusDiagnosticsSnapshot`, and target-aware relationship/blocker digests plus decision summaries.
- Added popup-stack diagnostics exposure through `QApplication::btkPopupStackDiagnostics()` and propagated it into BTK diagnostics/overlay tooling, including filtered focus-owner popup-stack views.
- Refined popup-aware wheel-event gating so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware context-menu routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware drag/drop routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware help/tooltip routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware tablet routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware touch routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware gesture routing so same-owner widgets can continue routing under BTK popup allowance rules.
- Refined popup-aware mouse enter/leave receiver tracking so unrelated-owner widgets are not blindly promoted while a popup is active.
- Refined popup-aware synthetic enter/leave generation so unrelated-owner widgets are not blindly targeted while a popup is active.
- Added a live-repository BTK vs BobUI comparison doc grounded in the current `workspace/bobui` repo, including findings about Qt-OSS vs CopperSpice lineage, OmniUI implementation gaps, Go/WebView bridge strengths, and why BTK is the better long-term framework mainline.
- Added focus/target popup-relationship summaries and popup-aware inspector-style HUD detail.
- Added `scripts/generate_btk_api_inventory.py` and generated rename inventory artifacts.
- Rewrote the root `README.md` around BTK.
- Added and expanded `docs/ai/` requirements/design/planning/implementation/testing docs, including deep comparison docs, rename buckets, package smoke examples, multi-user scaffolding analysis, and Windows build-enablement findings.

## Key Findings
- Current BTK has a mature C++ runtime base but lacks a verified modern declarative/web runtime story.
- BobUI is useful for multiplayer ownership ideas and WebView bridge concepts, but much of it remains partial/aspirational.
- JUCE is the strongest reference for audio/DSP/plugin work.
- Ultimate++ is the strongest reference for RAD/productivity subsystems.
- Windows/MSVC configuration is now working; `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, `CsMultimedia`, and `CsWebKit` have been built successfully in `Release` under `build-vs2019`.
- A staged Windows install now succeeds under `build-vs2019/install`, including BTK package config files.
- Windows package metadata is now duplicated into conventional `lib/cmake/BTK` and `lib/cmake/CopperSpice` locations so root-prefix `CMAKE_PREFIX_PATH` discovery works in addition to explicit `BTK_DIR`.
- The downstream `find_package(BTK)` core smoke example now configures, builds, and runs successfully against the staged install.
- A downstream GUI-oriented `find_package(BTK)` smoke example now configures, builds, and runs successfully against the staged install.
- A downstream Network-oriented `find_package(BTK)` smoke example now configures, builds, and runs successfully against the staged install.
- Downstream OpenGL- and Svg-oriented `find_package(BTK)` smoke examples now configure, build, and run successfully against the staged install.
- Downstream Sql- and Multimedia-oriented `find_package(BTK)` smoke examples now configure, build, and run successfully against the staged install.
- A richer downstream runtime-oriented GUI `find_package(BTK)` smoke example now configures, builds, and runs successfully against the staged install, including owner-exclusive rejection diagnostics through `BTKFocusOverlay`.
- A downstream integrated runtime `find_package(BTK)` smoke example now configures, builds, and runs successfully against the staged install, exercising Core + Gui + Network + Svg together with BTK-specific multi-owner diagnostics.
- A broader downstream platform-smoke `find_package(BTK)` example now configures, builds, and runs successfully against the staged install, exercising Core + Gui + Network + OpenGL + Svg + Sql + Multimedia together with BTK-specific multi-owner diagnostics.
- A downstream behavioral-runtime `find_package(BTK)` example now configures, builds, and runs successfully against the staged install, validating same-owner transfer vs cross-owner rejection behavior through BTK-specific diagnostics.
- A downstream focus-reason `find_package(BTK)` example now configures, builds, and runs successfully against the staged install, validating popup-reason and active-window-reason routing decisions via BTK-specific APIs.
- A downstream popup/modal `find_package(BTK)` example now configures, builds, stages, and runs successfully against the staged install, validating visible Windows GUI deployment plus BTK popup/modal ownership diagnostics.
- A downstream popup-stack runtime `find_package(BTK)` example now configures, builds, stages, and runs successfully against the staged install, validating visible multi-popup stack diagnostics plus popup-aware focus rejection for a foreign-owner target.
- The staged BTK install is sufficient for downstream linking, but visible Windows GUI executables still need an app-local `platforms/CsGuiWin2.1.dll` deployment layout.
- `cmake/BTKConfig.cmake` now mirrors BTK metadata into legacy deploy variables so `include(BTKDeploy)` plus `btk_copy_library(...)` / `btk_copy_plugins(...)` work in downstream projects, and it prepends the BTK package CMake directory to `CMAKE_MODULE_PATH` so `include(BTKDeploy)` resolves naturally after `find_package(BTK)`.
- A more ambitious restoration-focused popup-stack variant reproducibly hit a Windows access violation when programmatically closing the top popup in a visible staged GUI scenario, identifying popup close/restoration as a remaining runtime hotspot.
- The current BML substrate is legacy `QtDeclarative` / QML1-shaped and still depends structurally on missing `QtScript` / `QScript*` infrastructure, so BML is currently a branding/bootstrap layer rather than a fully buildable runtime.
- The generated dependency inventory shows the gap is broad rather than isolated: `src/declarative/` has 44 files with QtScript-related includes, repeated use of private `qscriptdeclarativeclass_p.h`/`QScriptDeclarativeClass` bridge machinery, and all four `src/imports/` plugin directories are declarative-coupled.
- The generated Script manifest refines that further: only 5 declarative public headers directly expose Script coupling, while 25 private headers depend on Script internals, making a staged public-compatibility checkpoint plausible before full declarative bridge revival.
- Recent BTK additions needed CopperSpice-compatible cleanup (`formatArg`, `QFlags` aliases, QString-based property keys, older `QFontMetrics` APIs) to compile cleanly.

## Recommended Next Steps
1. Expand the downstream BTK package smoke path beyond the current core/gui/network/opengl/svg/sql/multimedia/runtime/integrated/platform/behavioral/focus-reason/popup-modal/popup-stack validations into richer runtime-oriented consumption examples.
2. Continue the new BML bootstrap from naming compatibility into an actually buildable declarative runtime strategy, especially around the missing `QtScript`/`QScript*` dependency story in `src/declarative`.
3. Decide whether BML should revive the legacy declarative engine via a restored Script module, or whether BTK should use a hybrid revival plan that modernizes behind the BML name in stages.
4. Turn the new Script manifest into a concrete file/type checklist for the first `CsScript` bring-up pass, split into public compatibility headers and private declarative bridge support.
5. Expand the public alias layer cautiously based on validation feedback and reduce remaining CopperSpice-shaped API surprises for downstream users.
6. Continue evolving `BTKFocusOverlay` from a lightweight HUD toward a richer inspector-like multi-panel developer tool with deeper interaction, stronger owner/blocker grouping, blocked-reason clustering, blocker drilldown, mismatch-focused inspection, popup-stack inspection, popup-relationship inspection, and more precise blocked-route visualization, while refining mixed-owner popup behavior.
7. Continue the subsystem gap matrix into concrete implementation checklists for Qt6/JUCE/U++/BobUI/JavaFX/ImGui.

## Validation / Blockers
- Windows CMake configure now succeeds with Visual Studio 2019 Build Tools using `-G "Visual Studio 16 2019" -A x64`.
- `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, `CsMultimedia`, `CsWebKit`, `uic`, and `rcc` have been built successfully in `Release` under `build-vs2019`.
- A staged install now succeeds under `build-vs2019/install`.
- A downstream `find_package(BTK)` smoke example now configures, builds, and runs successfully against that staged install.
- Remaining validation is now about broader downstream/package coverage and optional/deferred module completion, not total compiler absence.

## Not Done
- Full repo-wide `Cs*` symbol migration.
- Full BML/QML runtime revival or modernization beyond the current BML bootstrap naming layer.
- QML/Quick/QuickControls2/WebEngineQuick implementation.
- Full assimilation of BobUI/JUCE/U++ feature sets.
- Full end-to-end `Release` build of all remaining modules in a single pass.
- Richer downstream GUI/package smoke validation beyond the current core/gui/network/opengl/svg/sql/multimedia/runtime/integrated/platform/behavioral/focus-reason/popup-modal/popup-stack samples.
