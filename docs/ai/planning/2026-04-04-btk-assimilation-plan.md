# BTK Assimilation Plan

## Phase 0 - Bootstrap (completed here)
- [x] Add reference submodules: JUCE, Ultimate++, BobUI.
- [x] Rebrand top-level project metadata to BTK.
- [x] Add `find_package(BTK)` support.
- [x] Capture initial requirements/design/testing docs.

## Phase 1 - Rename and Compatibility
- [ ] Inventory every public `CopperSpice` / `Cs*` symbol.
- [ ] Classify into safe alias, risky rename, and ABI-sensitive buckets.
- [ ] Introduce `Btk*` compatibility aliases for top-level library targets and selected public APIs.
- [ ] Add automated checks preventing mixed/accidental naming regressions.

## Phase 2 - Gap Audit
- [ ] Produce subsystem-by-subsystem inventory for current BTK.
- [ ] Map BobUI features into: adopt, reinterpret, reject.
- [ ] Map JUCE modules into: audio, DSP, plugin host/client, graphics, animation, JS, video.
- [ ] Map Ultimate++ subsystems into: CtrlCore/CtrlLib, docking, grid, report, IDE/tooling, rich text, SQL/UI.
- [ ] Add Qt 5/6/JavaFX/ImGui feature matrix.

## Phase 3 - Multiplayer Foundations
- [ ] Introduce `InputOwner`, `FocusToken`, `SurfaceOwnershipPolicy`, and event arbitration primitives.
- [ ] Refactor modal semantics to owner-scoped modality.
- [ ] Add tests for concurrent focus ownership.

## Phase 4 - Declarative/UI Runtime
- [ ] Decide whether to revive/modernize existing `src/declarative` or create a new BTK declarative runtime.
- [ ] Define QML/Quick parity targets.
- [ ] Plan QuickControls2-equivalent control library.
- [ ] Define modern web embedding architecture beyond legacy WebKit.

## Phase 5 - Media / DSP / Plugin Assimilation
- [ ] Introduce audio graph and DSP layers influenced by JUCE.
- [ ] Plan plugin hosting/client interfaces.
- [ ] Define realtime-safe threading and device abstraction rules.

## Phase 6 - Productivity / RAD Assimilation
- [ ] Study Ultimate++ data grid, docking, report, and IDE-oriented features.
- [ ] Define BTK equivalents with improved API consistency.

## Phase 7 - Tooling and Immediate Mode
- [ ] Add developer overlay, inspection tooling, and Dear ImGui-style debug surfaces.

## Rule
No broad parity claim is considered complete until it has build validation, test coverage, and a concrete feature checklist.
