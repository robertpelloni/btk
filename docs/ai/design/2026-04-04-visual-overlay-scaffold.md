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
- auto-refreshing text snapshot
- optional target widget for deeper context
- built directly on `BtkFocusDiagnostics`

## Future directions
- richer styling and layout
- token tables / color coding
- per-owner cursor/focus panels
- modal/popup block reason visualization
- integration with immediate-mode debug surfaces
