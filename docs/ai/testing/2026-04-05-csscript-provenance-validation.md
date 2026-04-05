# CsScript Provenance Validation — 2026-04-05

## Validation performed
### Process audit
Ran a process listing before deeper repository/history inspection.
No processes were killed.

### Git history audit
Validated through git history that the repository previously contained:
- `src/script/`
- `src/scripttools/`
- CMake build files for both

### Key commits confirmed
- `4acd0d71e423cf10e8d3ce2f8b7fd7dcd13536e8` — full Script subtree present in historical import
- `fd59ad8fdcef85ee9517e7b768f0b7f5178bc68e` — Script CMake build support added
- `24653d638c507b5718d97bb37dc8b9b15b5dadf9` — ScriptTools CMake build support added
- `4fb8e3486b0c63cefaeeb2e734a6b6b7762b3170` — Script build files removed
- `9a48596c0c14b34140e23903feae02a430335f32` — Script source files removed
- `b71883ac547e1e62b0851c43c926ab2a8e319066` — ScriptTools build files removed
- `60e2952b94841a5274ec67caeae56f12a4ff5a89` — ScriptTools source removed

### Third-party path audit
Validated that historical Script CMake expected:
- `src/3rdparty/javascriptcore/...`

Current tree instead exposes JavaScriptCore through WebKit paths such as:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

## Conclusion
BTK already contains recoverable Script provenance internally.
The main restoration risk is therefore not missing source history, but adapting the recovered module to the current third-party layout and modern build graph.
