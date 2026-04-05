# CsScript Stage A Recovery Validation — 2026-04-05

## Validation performed
### Process audit
Ran a process listing before repository modification and validation.
No processes were killed.

### Default configure validation
Command:
```powershell
cmake -S . -B build-vs2019-stagea-script -G "Visual Studio 16 2019" -A x64
```

Result:
- configure succeeded
- existing default BTK module set remained healthy
- restoring `src/script/` did not regress the current default configure path

### Explicit Script probe validation
Command:
```powershell
cmake -S . -B build-vs2019-script-probe -G "Visual Studio 16 2019" -A x64 -DWITH_SCRIPT=ON
```

Result:
- configure succeeded
- BTK emitted a clear warning
- `WITH_SCRIPT` was forced back to `OFF` for that configure pass

### Warning meaning confirmed
The warning correctly identified that the restored historical `CsScript` baseline still expects JavaScriptCore-era files absent from the current WebKit layout, including representative gaps such as:
- legacy `pcre` sources
- legacy `wrec` sources
- legacy runtime files such as `Collector.cpp`

## Conclusion
The Stage A bootstrap pass succeeded in its immediate goals:
- restore historical Script sources into the working tree
- preserve default configure health
- replace opaque future failure with an explicit configure-time warning for Script enablement attempts

The next recovery step is now focused adaptation work against the current WebKit JavaScriptCore layout, not source provenance or baseline ambiguity.
