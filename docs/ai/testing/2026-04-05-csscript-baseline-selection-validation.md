# CsScript Baseline Selection Validation — 2026-04-05

## Validation performed
### Selected Script baseline
Confirmed that:
- `85fd29b0d114fccb7b8eca371614bbb813ac0a01`

contains a complete `src/script/` snapshot including:
- source files
- `src/script/CMakeLists.txt`
- `api.cmake`
- `bridge.cmake`
- `parser.cmake`

### Selected ScriptTools baseline
Confirmed that:
- `9845b18dcafa2a4552f394a85e70c43f18a8b797`

contains a complete `src/scripttools/` snapshot including source and build files.

### Removal chronology confirmed
Validated that:
- `4fb8e3486b0c63cefaeeb2e734a6b6b7762b3170` removed Script build files
- `9a48596c0c14b34140e23903feae02a430335f32` removed Script source files
- `b71883ac547e1e62b0851c43c926ab2a8e319066` removed ScriptTools build files
- `60e2952b94841a5274ec67caeae56f12a4ff5a89` removed ScriptTools source files

### Path-layout delta confirmed
Validated that the selected historical Script baseline expected:
- `src/3rdparty/javascriptcore/...`

while the current tree now exposes JavaScriptCore through:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

## Conclusion
The selected baseline commits are valid recovery anchors.
The next restoration risk is no longer baseline ambiguity, but path/build adaptation during reintroduction into the current BTK tree.
