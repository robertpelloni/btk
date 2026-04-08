#!/usr/bin/env python3
"""
BTK Naming Consistency Checker
Validates that no CopperSpice references remain in BTK source files
(excluding known backward-compat layers and third-party code).
"""

import os
import sys
import re

# Directories to skip entirely
SKIP_DIRS = {
    'external', '3rdparty', 'webkit', 'build-vs2019',
    'build-vs2019-script-probe', 'build-vs2019-script-probe2',
    'build-vs2019-script-probe3', 'build-vs2019-script-probe4',
    'build-vs2019-script-probe5', 'build-vs2019-script-probe6',
    'build-vs2019-stagea-script', 'build-vs2019-declarative-probe',
}

# Files that are expected to contain CopperSpice references (legacy compat)
ALLOWED_FILES = {
    'cmake/CopperSpiceConfig.cmake',
    'cmake/CopperSpiceConfigVersion.cmake',
    'cmake/CopperSpiceDeploy.cmake',
    'cmake/CopperSpiceMacros.cmake',
    'cmake/BTKConfig.cmake',        # references CopperSpice:: targets
    'cmake/BTKDeploy.cmake',        # references CopperSpiceDeploy.cmake
    'cmake/BTKMacros.cmake',        # references CopperSpiceMacros.cmake
    'scripts/generate_btk_api_inventory.py',  # inventories Cs* naming
}

# Extensions to check
CHECK_EXTENSIONS = {'.h', '.cpp', '.cmake', '.txt', '.py', '.ts'}

def should_check(filepath):
    """Determine if a file should be checked."""
    # Skip if in a skip directory
    parts = filepath.replace('\\', '/').split('/')
    for skip_dir in SKIP_DIRS:
        if skip_dir in parts:
            return False

    # Skip if it's an allowed legacy file
    for allowed in ALLOWED_FILES:
        if filepath.replace('\\', '/').endswith(allowed):
            return False

    # Check extension
    _, ext = os.path.splitext(filepath)
    return ext in CHECK_EXTENSIONS

def check_file(filepath):
    """Check a single file for forbidden patterns."""
    violations = []

    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except Exception:
        return violations

    # Patterns that should NOT appear in non-legacy BTK source
    forbidden = [
        (r'\bThis file is part of CopperSpice\b', 'CopperSpice copyright header'),
        (r'\bCopperSpice is free software\b', 'CopperSpice license text'),
        (r'\bCopperSpice is distributed\b', 'CopperSpice disclaimer'),
    ]

    for pattern, description in forbidden:
        matches = re.finditer(pattern, content)
        for match in matches:
            line_num = content[:match.start()].count('\n') + 1
            violations.append((filepath, line_num, description))

    return violations

def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    os.chdir(root)

    all_violations = []
    files_checked = 0

    for dirpath, dirnames, filenames in os.walk('.'):
        # Skip hidden and build directories
        dirnames[:] = [d for d in dirnames if not d.startswith('.')]

        for filename in filenames:
            filepath = os.path.join(dirpath, filename)

            if should_check(filepath):
                violations = check_file(filepath)
                all_violations.extend(violations)
                files_checked += 1

    print(f"BTK Naming Consistency Check")
    print(f"Files checked: {files_checked}")
    print(f"Violations found: {len(all_violations)}")
    print()

    if all_violations:
        print("VIOLATIONS:")
        for filepath, line_num, description in all_violations:
            print(f"  {filepath}:{line_num} - {description}")
        print()
        print("FAIL: Naming consistency violations detected.")
        sys.exit(1)
    else:
        print("PASS: All non-legacy source files use BTK branding.")
        sys.exit(0)

if __name__ == '__main__':
    main()
