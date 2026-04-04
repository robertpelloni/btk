# BTK Alias Layer - Session Notes

## What was added
### CMake package wrappers
- `cmake/BTKConfig.cmake`
- `cmake/BTKConfigVersion.cmake`
- `cmake/BTKMacros.cmake`
- `cmake/BTKDeploy.cmake`

### New BTK conveniences
For each built framework component, the project now exposes both:
- `BTK::Btk<Component>`
- `BTK::<Component>`

Examples:
- `BTK::BtkCore`
- `BTK::Core`
- `BTK::BtkGui`
- `BTK::Gui`

These are compatibility aliases over the existing exported `CopperSpice::Cs*` targets.

### Wrapper APIs
- `BTK_RESOURCES(...)` forwards to `COPPERSPICE_RESOURCES(...)`
- `btk_copy_library(...)` forwards to `cs_copy_library(...)`
- `btk_copy_plugins(...)` forwards to `cs_copy_plugins(...)`

## Why this matters
This creates a safe migration path:
1. downstreams can begin writing BTK-branded CMake today
2. the existing internal/export target graph remains stable
3. broad symbol/file renames can happen later in controlled phases

## What this does NOT do
- It does not rename ABI symbols.
- It does not rename headers or source files.
- It does not remove `CopperSpice::Cs*` compatibility.
- It does not yet add `Btk*` C++ type aliases across the public headers.

## Recommended next implementation step
Use the generated rename inventory to introduce carefully-scoped public header aliases for the safest high-value API surfaces first.
