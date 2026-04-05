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
- Added Windows build-enablement findings documenting successful MSVC configuration plus successful `CsCore`, `CsXml`, `CsGui`, `CsNetwork`, `CsOpenGL`, `CsSql`, `CsSvg`, `CsXmlPatterns`, `CsMultimedia`, and `CsWebKit` `Release` builds under `build-vs2019`, along with a staged install, root-prefix package discovery via `lib/cmake/BTK`, and successful downstream core/gui/network/opengl/svg `find_package(BTK)` smoke validation.

## Strategic assessment
The requested end-state (full parity with modern Qt/JUCE/GTK/JavaFX/ImGui plus BobUI improvements) is a long multi-phase program, not a single-session edit. The safest path is:
- compatibility-first renaming
- capability-matrix-based planning
- subsystem-by-subsystem implementation and validation

## Immediate technical follow-up
- Expand downstream BTK package validation beyond the now-working core/gui/network/opengl/svg smoke paths into richer GUI/package consumption examples.
- Continue the successful Windows/MSVC build path into any deferred optional modules/plugins beyond the current built set.
- Validate the first-wave BTK public C++ aliases and multi-user scaffolding with a real downstream build in a compiler-ready environment.
- Continue evolving `BTKFocusOverlay` from a lightweight HUD toward a richer inspector-like multi-panel developer tool with deeper interaction, stronger owner/blocker grouping, blocked-reason clustering, blocker drilldown, mismatch-focused inspection, popup-stack inspection, popup-relationship inspection, and more precise blocked-route visualization, while refining mixed-owner popup behavior.
- Decide whether existing `src/declarative` is salvageable or whether a new BTK declarative runtime is cleaner.
