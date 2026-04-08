# BTK Phase 2 Implementation Plan

## Phase 2A - Core API Modernization (Current Priority)

### Objectives
Complete the CopperSpice → BTK rebrand and establish clean public API surface.

### Tasks
- [x] Rename all source file comments from CopperSpice to BTK
- [x] Rename all user-facing strings from CopperSpice to BTK
- [x] Add comprehensive BTK subsystem alias headers
- [x] Update cmake package paths (BTK primary, CopperSpice legacy)
- [ ] Validate build compiles cleanly after rebrand
- [ ] Add automated naming regression check script
- [ ] Complete BTK public class aliases for all major types:
  - [ ] `BTKApplication` → `QApplication`
  - [ ] `BTKWidget` → `QWidget`
  - [ ] `BTKWindow` → `QMainWindow`
  - [ ] `BTKButton` → `QPushButton`
  - [ ] `BTKLabel` → `QLabel`
  - [ ] `BTKLineEdit` → `QLineEdit`
  - [ ] `BTKTextEdit` → `QTextEdit`
  - [ ] `BTKListView` → `QListView`
  - [ ] `BTKTreeView` → `QTreeView`
  - [ ] `BTKTableView` → `QTableView`
  - [ ] `BTKLayout` → `QLayout`
  - [ ] `BTKTimer` → `QTimer`
  - [ ] `BTKThread` → `QThread`
  - [ ] `BTKSocket` → `QTcpSocket`
  - [ ] `BTKDatabase` → `QSqlDatabase`
  - [ ] `BTKPainter` → `QPainter`
  - [ ] `BTKColor` → `QColor`
  - [ ] `BTKImage` → `QImage`
  - [ ] `BTKPixmap` → `QPixmap`
  - [ ] `BTKFont` → `QFont`
  - [ ] `BTKPoint` → `QPoint`
  - [ ] `BTKRect` → `QRect`
  - [ ] `BTKSize` → `QSize`

### Phase 2A Completion Criteria
- Build compiles on Windows/MSVC with zero errors
- All BTK alias headers install correctly
- `find_package(BTK)` works for downstream consumers
- Naming regression check passes

---

## Phase 2B - Accessibility (Critical Gap)

### Objectives
Implement accessibility framework matching Qt 6 / WinUI 3 / GTK 4 level.

### Tasks
- [ ] Define `BtkAccessible` interface
- [ ] Add accessible role/name/description properties to widgets
- [ ] Implement MSAA/UIA bridge for Windows
- [ ] Implement ATK bridge for Linux
- [ ] Implement NSAccessibility bridge for macOS
- [ ] Add screen reader event notifications
- [ ] Add keyboard navigation enhancement

---

## Phase 2C - Scene Graph and RHI

### Objectives
Modern rendering pipeline with hardware abstraction.

### Tasks
- [ ] Define `BtkSceneGraph` node tree
- [ ] Implement `BtkRhiBackend` abstraction layer
- [ ] Add OpenGL RHI backend
- [ ] Add Vulkan RHI backend
- [ ] Add Direct3D RHI backend (Windows)
- [ ] Add Metal RHI backend (macOS)
- [ ] Integrate with existing widget rendering

---

## Phase 2D - Declarative UI Runtime (BML)

### Objectives
Revive or create a modern declarative UI runtime.

### Tasks
- [ ] Complete CsScript Stage A link recovery
- [ ] Revive BML declarative engine through CsScript restoration
- [ ] Define BML language syntax (QML-compatible or BTK-native)
- [ ] Implement property binding engine
- [ ] Add CSS-like styling support
- [ ] Build BML Designer tool

---

## Phase 2E - Audio/DSP/Plugin Stack

### Objectives
JUCE-competitive audio processing capabilities.

### Tasks
- [ ] Define `BtkAudioDevice` abstraction
- [ ] Implement `BtkAudioGraph` processing pipeline
- [ ] Add `BtkDsp` primitives (filters, FFT, oscillators)
- [ ] Implement MIDI I/O (`BtkMidiInput`, `BtkMidiOutput`)
- [ ] Add VST3/AU plugin host
- [ ] Build `BtkAudioEditor` base component

---

## Phase 2F - Productivity/RAD Widgets

### Objectives
Ultimate++-competitive productivity widget suite.

### Tasks
- [ ] Implement `BtkDockPanel` docking window system
- [ ] Add `BtkRichTextEdit` rich text editor
- [ ] Add `BtkCodeEditor` source code editor
- [ ] Add `BtkChart` data visualization
- [ ] Add `BtkReport` report generation
- [ ] Add `BtkPdfWriter` PDF output
- [ ] Add `BtkPropertyGrid` property inspection widget

---

## Phase 2G - Modern Web Integration

### Objectives
Replace legacy WebKit with modern web engine.

### Tasks
- [ ] Evaluate WebView2 (Windows) / WKWebView (macOS) / WebKitGTK (Linux)
- [ ] Define `BtkWebView` widget interface
- [ ] Implement JavaScript bridge
- [ ] Add devtools integration

---

## Phase 2H - Developer Tooling

### Objectives
Best-in-class developer experience.

### Tasks
- [ ] Build `BtkInspector` runtime widget inspector
- [ ] Add Dear ImGui-style immediate mode debug layer
- [ ] Add performance profiling hooks
- [ ] Build BTK Designer IDE (long-term)
