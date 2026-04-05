# BML Bootstrap Requirements — 2026-04-05

## Goal
Introduce a BTK-native declarative naming layer where QML-style capabilities are branded as **BML**.

## Immediate requirement
The user asked to implement QML in BTK but call it BML.

Because the repository already contains a substantial legacy `src/declarative/` tree, the safest first implementation step is:
- do **not** invent a second declarative runtime immediately
- expose a **BML-branded compatibility/bootstrap layer** over the existing declarative substrate
- keep the declarative module build disabled by default until its missing dependency and modernization questions are resolved

## Functional requirements for this bootstrap step
- Add BTK/BML branding at the declarative API layer.
- Introduce public BML C++ aliases for the most central declarative concepts.
- Introduce a `bmlRegisterType(...)` family mirroring `qmlRegisterType(...)`.
- Add a `BTK::Bml` target alias when the declarative module is built.
- Add a build gate for `Declarative` that is present in the top-level project but disabled by default.

## Constraints
- The existing declarative implementation is legacy `QtDeclarative` / QML1-shaped, not modern Qt Quick 2 / QML 2.
- `src/declarative` currently includes hard references to `QtScript` / `QScript*` APIs, while a top-level `CsScript` module is not currently built in this repo.
- Therefore full "modern BML runtime" delivery is **not** a single-pass feature in this session.

## Non-functional requirements
- Preserve current default buildability.
- Avoid enabling a broken declarative build path by default.
- Document clearly that this is a compatibility/bootstrap pass, not full BML parity.
