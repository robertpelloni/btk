from __future__ import annotations

import json
import os
import re
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DECLARATIVE_ROOT = ROOT / "src" / "declarative"
IMPORTS_ROOT = ROOT / "src" / "imports"
DOC_MD = ROOT / "docs" / "ai" / "implementation" / "2026-04-05-bml-qtscript-dependency-audit.md"
DOC_JSON = ROOT / "docs" / "ai" / "implementation" / "2026-04-05-bml-qtscript-dependency-audit.json"

TEXT_SUFFIXES = {".h", ".hpp", ".hh", ".hxx", ".c", ".cc", ".cpp", ".cxx", ".cmake", ".txt"}
SCRIPT_INCLUDE_PATTERNS = [
    re.compile(r"#include\s+<([^>]*QtScript/[^>]+)>") ,
    re.compile(r"#include\s+<([^>]*qscript[^>]+)>") ,
]
SCRIPT_IDENTIFIERS = [
    "QScriptValue",
    "QScriptEngine",
    "QScriptContext",
    "QScriptContextInfo",
    "QScriptEngineAgent",
    "QScriptProgram",
    "QScriptString",
    "QScriptClass",
    "QScriptClassPropertyIterator",
    "QScriptValueIterator",
    "QScriptDeclarativeClass",
]
IMPORT_DECLARATIVE_PATTERN = re.compile(r"QDeclarative|qdeclarative|qmlRegister|QML_DECLARE_TYPE|QDeclarativeExtensionPlugin")


def iter_text_files(root: Path):
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            path = Path(dirpath) / filename
            if path.suffix.lower() in TEXT_SUFFIXES or filename == "CMakeLists.txt":
                yield path


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="ignore")
    except OSError:
        return ""


def main() -> None:
    include_counts: Counter[str] = Counter()
    include_files: dict[str, set[str]] = defaultdict(set)
    files_with_script_includes: list[dict[str, object]] = []
    identifier_counts: dict[str, dict[str, int]] = {}
    script_hits_by_dir: Counter[str] = Counter()

    for path in iter_text_files(DECLARATIVE_ROOT):
        rel = path.relative_to(DECLARATIVE_ROOT).as_posix()
        text = read_text(path)
        if not text:
            continue

        matches: list[str] = []
        for pattern in SCRIPT_INCLUDE_PATTERNS:
            matches.extend(pattern.findall(text))

        if matches:
            unique_matches = sorted(set(matches))
            files_with_script_includes.append(
                {
                    "file": rel,
                    "includes": unique_matches,
                    "include_count": len(matches),
                }
            )
            top_dir = rel.split("/", 1)[0]
            script_hits_by_dir[top_dir] += len(matches)
            for match in matches:
                include_counts[match] += 1
                include_files[match].add(rel)

    for identifier in SCRIPT_IDENTIFIERS:
        rx = re.compile(r"\b" + re.escape(identifier) + r"\b")
        file_count = 0
        hit_count = 0
        for path in iter_text_files(DECLARATIVE_ROOT):
            text = read_text(path)
            if not text:
                continue
            hits = len(rx.findall(text))
            if hits:
                file_count += 1
                hit_count += hits
        identifier_counts[identifier] = {
            "files": file_count,
            "hits": hit_count,
        }

    import_plugin_hits: list[dict[str, object]] = []
    if IMPORTS_ROOT.exists():
        for child in sorted(IMPORTS_ROOT.iterdir()):
            if not child.is_dir():
                continue
            hits = 0
            files = 0
            for path in iter_text_files(child):
                text = read_text(path)
                if not text:
                    continue
                count = len(IMPORT_DECLARATIVE_PATTERN.findall(text))
                if count:
                    hits += count
                    files += 1
            if hits:
                import_plugin_hits.append(
                    {
                        "directory": child.name,
                        "files": files,
                        "hits": hits,
                    }
                )

    data = {
        "root": str(ROOT),
        "declarative_root": str(DECLARATIVE_ROOT),
        "imports_root": str(IMPORTS_ROOT),
        "files_with_qtscript_includes": len(files_with_script_includes),
        "unique_qtscript_include_headers": len(include_counts),
        "top_qtscript_includes": [
            {
                "header": header,
                "references": count,
                "files": len(include_files[header]),
            }
            for header, count in include_counts.most_common()
        ],
        "qtscript_identifier_counts": identifier_counts,
        "script_hits_by_declarative_subdir": dict(script_hits_by_dir.most_common()),
        "files_with_script_includes": sorted(files_with_script_includes, key=lambda item: item["file"]),
        "imports_declarative_dependency_summary": import_plugin_hits,
    }

    DOC_JSON.parent.mkdir(parents=True, exist_ok=True)
    DOC_JSON.write_text(json.dumps(data, indent=2), encoding="utf-8")

    lines: list[str] = []
    lines.append("# BML QtScript Dependency Audit\n")
    lines.append("This document is generated by `scripts/generate_bml_qtscript_dependency_audit.py`. It inventories the QtScript/CsScript-shaped dependency footprint inside BTK's legacy declarative stack.\n")
    lines.append("## Headline counts\n")
    lines.append(f"- Files in `src/declarative/` with QtScript-related includes: {len(files_with_script_includes)}")
    lines.append(f"- Unique QtScript-related include headers referenced: {len(include_counts)}")
    lines.append(f"- Import plugin directories under `src/imports/` with declarative coupling: {len(import_plugin_hits)}")
    lines.append("")
    lines.append("## Top QtScript include headers\n")
    lines.append("| Header | References | Files |")
    lines.append("|---|---:|---:|")
    for item in data["top_qtscript_includes"][:30]:
        lines.append(f"| `{item['header']}` | {item['references']} | {item['files']} |")
    lines.append("")
    lines.append("## QtScript identifier footprint\n")
    lines.append("| Identifier | Files | Hits |")
    lines.append("|---|---:|---:|")
    for identifier in SCRIPT_IDENTIFIERS:
        counts = identifier_counts[identifier]
        lines.append(f"| `{identifier}` | {counts['files']} | {counts['hits']} |")
    lines.append("")
    lines.append("## Script include pressure by declarative subdirectory\n")
    lines.append("| Subdirectory | Include hits |")
    lines.append("|---|---:|")
    for directory, count in script_hits_by_dir.most_common():
        lines.append(f"| `{directory}` | {count} |")
    lines.append("")
    lines.append("## `src/imports/` declarative dependency summary\n")
    if import_plugin_hits:
        lines.append("| Directory | Files with declarative coupling | Match hits |")
        lines.append("|---|---:|---:|")
        for item in import_plugin_hits:
            lines.append(f"| `{item['directory']}` | {item['files']} | {item['hits']} |")
    else:
        lines.append("- No declarative-dependent import plugin directories were detected.")
    lines.append("")
    lines.append("## Interpretation\n")
    lines.append("- The dependency is broad, not isolated to a single file or helper. QtScript is woven through public API, engine internals, utility models, debugger helpers, and script-class abstractions.")
    lines.append("- `QScriptDeclarativeClass` and `qscriptdeclarativeclass_p.h` are especially important because they reflect use of private script/declarative bridge infrastructure, not just public script APIs.")
    lines.append("- `src/imports/` also carries declarative-era plugin payloads, so a future BML revival likely needs to account for more than just `src/declarative/`.")
    lines.append("- The current BML bootstrap should therefore remain branding/package-oriented until a Script revival or replacement plan exists.\n")

    DOC_MD.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
