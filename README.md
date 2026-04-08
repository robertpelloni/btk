## BTK

### Introduction

BTK is a cross-platform C++ application framework descended from the CopperSpice/Qt lineage, being actively reworked into the definitive systems foundation for large-scale software and operating-system development. The goal is to achieve 1:1 feature parity with Qt 6/7, JavaFX, JUCE, GTK, WinUI, and Ultimate++, while establishing BTK as the greatest windowing toolkit with the smoothest, most robust API and documentation.

BTK is intended to become a foundational runtime for a new OS/application stack where multiple local and remote users can interact with software concurrently.

### Architecture

BTK provides a mature, modular C++ framework organized into the following subsystems:

| Module | Description |
|--------|-------------|
| **BtkCore** | Foundation types, object model, threading, I/O, timers, properties, signals/slots |
| **BtkGui** | Widgets, painting, input, windowing, focus management, multi-user ownership |
| **BtkNetwork** | TCP/UDP sockets, HTTP, SSL/TLS, DNS, network proxies |
| **BtkXml** | DOM and SAX XML parsing, stream readers/writers |
| **BtkSql** | Database access with driver plugins (PostgreSQL, MySQL, ODBC, SQLite, etc.) |
| **BtkSvg** | SVG rendering and display |
| **BtkOpenGL** | Cross-platform OpenGL rendering |
| **BtkVulkan** | Cross-platform Vulkan rendering |
| **BtkMultimedia** | Audio, video, camera, and media playback |
| **BtkWebKit** | Embedded web browser engine |
| **BtkXmlPatterns** | XQuery, XPath, and XML schema validation |
| **BtkBml** | Declarative UI runtime (BTK Markup Language) |

### Quick Start

```cpp
#include <btkcore.h>

int main(int argc, char *argv[])
{
   BTKApplication app(argc, argv);
   // Your application code here
   return app.exec();
}
```

### CMake Integration

```cmake
find_package(BTK REQUIRED Core Gui Network)

target_link_libraries(myapp
   BTK::Core
   BTK::Gui
   BTK::Network
)
```

BTK also provides compatibility with legacy CopperSpice naming:

```cmake
find_package(BTK REQUIRED)
# Both BTK:: and CopperSpice:: targets work
target_link_libraries(myapp BTK::Core)
```

### Multi-User Ownership Model

BTK introduces first-class multi-user input ownership:

- **BtkInputOwner** - Represents a local or remote user with device capabilities
- **BtkFocusToken** - Owner-scoped focus tokens for explicit focus management
- **BtkInputArbitrator** - Resolves competing input ownership claims
- **BTKFocusOverlay** - Developer overlay for debugging focus/ownership state

### BTK Public API Aliases

BTK provides clean branded aliases for all major types:

```cpp
#include <btkstring.h>    // BtkString, BtkChar, BtkStringUtf8, etc.
#include <btkpointer.h>   // BtkSharedPointer, BtkUniquePointer, etc.
#include <btksignal.h>    // BtkSignal::SignalBase, connection kinds, etc.
```

### Reference Submodules

The following repositories are tracked as reference inputs for ongoing assimilation work:

- `external/juce` - JUCE upstream (audio/DSP/plugin reference)
- `external/ultimatepp` - Ultimate++ upstream (productivity/RAD reference)
- `external/bobui-reference` - BobUI reference baseline (collaboration/ownership reference)

### Build Notes

BTK uses CMake as its primary build system:

```bash
cmake -G "Visual Studio 16 2019" -A x64 -B build
cmake --build build --config Release
cmake --install build --prefix install
```

Supported platforms: Windows (MSVC 2019+), macOS, Linux (GCC 7.3+ / Clang 6+), BSD.

### Research and Planning

Assimilation research, comparison matrices, and migration planning live under:

- `docs/ai/requirements/` - Problem understanding and requirements
- `docs/ai/design/` - System architecture and design decisions
- `docs/ai/planning/` - Task breakdown and project planning
- `docs/ai/implementation/` - Implementation guides and notes
- `docs/ai/testing/` - Testing strategy and test cases

### Status

This repository is in active development. The codebase contains mature subsystems with broad framework coverage. Multi-user ownership primitives are scaffolded and integrated into the GUI event routing layer. The larger parity goals with Qt 6+, JUCE, Ultimate++, JavaFX, and others are being approached systematically through phased assimilation.

### License

BTK is released under the GNU Lesser General Public License version 2.1.
