# BML Buildability Audit Validation — 2026-04-05

## Validation performed
### Top-level configure remains healthy
Validated:

```powershell
cmake -S . -B build-vs2019 -G "Visual Studio 16 2019" -A x64
```

Result:
- configure succeeds
- default BTK module set remains unchanged
- Declarative/BML stays disabled by default

### BML bootstrap did not regress staged GUI package flows
Revalidated downstream staged GUI package samples after the package/module-path improvement and BML bootstrap work:
- popup/modal staged sample
- popup-stack staged sample

Result:
- both still configure successfully
- both still build successfully
- both still stage successfully
- both still run successfully

### `include(BTKDeploy)` package ergonomics validated
After updating `BTKConfig.cmake` to prepend the BTK package directory into `CMAKE_MODULE_PATH`, the downstream staged GUI examples now work using:

```cmake
include(BTKDeploy)
```

instead of requiring the explicit full path to `BTKDeploy.cmake`.

## Conclusion
The BML bootstrap and buildability-guard work:
- improved declarative clarity
- preserved current build health
- improved downstream package ergonomics
- did not regress previously validated staged GUI deployment flows
