# 2026-04-04 GPT Bootstrap Handoff

This session established the first realistic groundwork for the user's very large BTK migration request.

## Repo changes
- Added submodules for JUCE, Ultimate++, and BobUI reference.
- Added BTK-branded CMake package wrappers and helper wrapper files.
- Added BTK alias targets in both `BTK::Btk<Component>` and `BTK::<Component>` forms.
- Added first-wave public C++ compatibility headers for BTK-branded aliases.
- Added first-wave multi-user ownership/focus scaffolding in `src/core/kernel` and `src/gui/kernel`.
- Added a first narrow real integration point in `QApplicationPrivate::setFocusWidget(...)` driven by BTK focus-token arbitration.
- Extended BTK ownership behavior into popup/modal gating paths.
- Added owner-aware BTK diagnostic/reporting helpers on `QApplication`.
- Extended popup stack restoration to prefer same-owner restoration semantics.
- Added `BTKFocusDiagnostics` as a reusable QtGui diagnostics adapter for future overlays.
- Added `BTKFocusOverlay` as the first visual BTK overlay scaffold.
- Refined `BTKFocusOverlay` into a richer HUD-style diagnostic surface with owner grouping, blocker grouping, relationship digest summaries, popup-stack summaries, blocked-route summaries, panel-based visibility control, preset-oriented panel modes, and blocked-only filtering.
- Refined `BTKFocusOverlay` further with a dedicated relationship panel, relationship-count summary chip, comparison-cluster summaries, target-owner comparison clusters, blocked-reason grouping, blocker drilldown grouping, mismatch highlighting, count-prioritized owner/blocker ordering, relationship digest extraction in `BtkFocusDiagnosticsSnapshot`, and target-aware relationship/blocker digests plus decision summaries.
- Added popup-stack diagnostics exposure through `QApplication::btkPopupStackDiagnostics()` and wired it into the overlay diagnostics path, including filtered focus-owner popup-stack views.
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
- Added a rename-inventory generator plus generated inventory artifacts.
- Updated root build branding and README.
- Added structured analysis/planning docs under `docs/ai/`.
- Added Windows build-enablement findings documenting successful MSVC configuration plus successful `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, `CsMultimedia`, and `CsWebKit` `Release` builds under `build-vs2019`, along with a staged install, root-prefix package discovery via `lib/cmake/BTK`, successful downstream core/gui/network/opengl/svg/sql/multimedia/runtime/integrated/platform/behavioral/focus-reason/popup-modal/popup-stack `find_package(BTK)` smoke validation, a BTK deploy-wrapper compatibility bridge for staged Windows GUI app bundles, a first BML branding/bootstrap pass over the legacy declarative subsystem, a declarative buildability audit/guard for missing QtScript-era dependencies, a generated QtScript dependency inventory quantifying the BML revival blast radius, and a generated Script manifest separating public Script-facing declarative headers from private bridge dependencies.

## Strategic assessment
The requested end-state (full parity with modern Qt/JUCE/GTK/JavaFX/ImGui plus BobUI improvements) is a long multi-phase program, not a single-session edit. The safest path is:
- compatibility-first renaming
- capability-matrix-based planning
- subsystem-by-subsystem implementation and validation

## Immediate technical follow-up
- Expand downstream BTK package validation beyond the now-working core/gui/network/opengl/svg/sql/multimedia/runtime/integrated/platform/behavioral/focus-reason/popup-modal/popup-stack smoke paths into richer GUI/package consumption examples.
- Continue the new BML bootstrap from naming compatibility into a buildable declarative/runtime revival plan, especially around the missing `QtScript`/`QScript*` dependency story in `src/declarative`.
- Decide whether BML should revive the legacy declarative engine via a restored Script module or move through a hybrid modernization plan behind the BML name.
- Use the generated QtScript dependency inventory and Script manifest to define the minimum viable `CsScript` file/type checklist before attempting declarative bring-up.
- The first-pass `CsScript` bring-up plan is now staged explicitly into public compatibility, declarative bridge/core compilation, and tooling/support follow-up.
- A provenance audit now shows the repository's own git history already contains recoverable `src/script/` and `src/scripttools/` snapshots, so the likely path is historical recovery plus JavaScriptCore path adaptation rather than a full greenfield Script rewrite.
- The concrete recovery anchors are now selected: use `85fd29...` as the primary `CsScript` baseline, defer `CsScriptTools` to `9845b18...`, and start with a Stage A public-compatibility restoration pass.
- Stage A has now begun concretely: `src/script/` was restored from `85fd29...`, `Script` was reintroduced as an optional top-level component, and configure-time guards now keep default build health intact while exposing the remaining JavaScriptCore drift explicitly when `WITH_SCRIPT=ON` is requested.
- A generated JavaScriptCore drift audit initially showed the restored Script baseline was mostly aligned with the current embedded tree (408 existing references vs 56 missing), which narrowed the work to clustered adaptation around `runtime`, `pcre`, `wrec`, and `yarr`.
- A follow-up contraction pass over `src/script/CMakeLists.txt` then reduced the restored Script JavaScriptCore path drift to zero missing references and allowed `WITH_SCRIPT=ON` to reach a successful experimental configure pass.
- The next pass then reached the first direct `CsScript` compiler diagnostics after restoring `Q_SCRIPT_EXPORT`, `SCRIPT_CS_*` helper aliases, and `qglobal.h` header visibility, confirming that the remaining Stage A challenge is JavaScriptCore private API drift rather than missing files or broken configure wiring.
- Continue the successful Windows/MSVC build path into any deferred optional modules/plugins beyond the current built set.
- Validate the first-wave BTK public C++ aliases and multi-user scaffolding with a real downstream build in a compiler-ready environment.
- Continue evolving `BTKFocusOverlay` from a lightweight HUD toward a richer inspector-like multi-panel developer tool with deeper interaction, stronger owner/blocker grouping, blocked-reason clustering, blocker drilldown, mismatch-focused inspection, popup-stack inspection, popup-relationship inspection, and more precise blocked-route visualization, while refining mixed-owner popup behavior.
- Decide whether existing `src/declarative` is salvageable or whether a new BTK declarative runtime is cleaner.
