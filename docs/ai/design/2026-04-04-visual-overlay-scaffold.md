# BTK Visual Overlay Scaffold

## Purpose
Introduce the first real visual developer-facing BTK overlay surface built on top of the new owner-aware diagnostics adapter.

## Added public surface
- `<QtGui/BTKFocusOverlay>`

## Added type
- `BtkFocusOverlay`

## Role
`BtkFocusOverlay` is a lightweight diagnostic widget that renders:
- current BTK focus diagnostics
- active popup/modal owner summaries
- token summaries
- optional target-widget path/context information

## Why this matters
This is the first step from:
- raw runtime concepts
- to diagnostics
- to a visible on-screen developer tool

It is intentionally modest. The goal is not final UX polish; the goal is to establish a real rendering surface that future BTK tooling can evolve from.

## Current behavior
- transparent-to-mouse overlay widget
- auto-refreshing overlay surface
- panel-based visibility control (`Summary`, `Focus`, `Owner`, `Token`, `Target`, `Blocked`, `Raw`)
- preset-oriented panel modes (`Compact`, `OwnerCentric`, `Analysis`, `Full`)
- preset cycling support and blocked-only filtering mode
- summary chips for popup owner, modal owner, refresh cadence, focus owner, focus surface, blocked-route count, and preset/mode state
- focus widget and path sections
- owner-group summary section
- blocker-group summary section
- token summary section with color-coded emphasis
- blocked/exclusive route section with more precise summaries
- optional target widget section with decision summary
- built directly on `BtkFocusDiagnostics`

## Current assessment
The overlay has now moved beyond a plain text dump into a richer HUD pass with owner grouping, blocked-route visualization, and basic panel-based behavior. It is still intentionally lightweight, but it already demonstrates the shape of a future BTK developer experience: runtime state transformed into a readable, always-on-top tool surface.

## Future directions
- richer styling and layout refinement
- interactive panel switching / controls
- click-driven or shortcut-driven panel interaction
- token tables / deeper color coding
- per-owner cursor/focus panels
- modal/popup block reason visualization
- integration with immediate-mode debug surfaces
