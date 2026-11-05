#!/usr/bin/env python3
"""Audit CPU scopes against the checked-in per-function coverage inventory."""
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent.parent


def functions(source):
    masked = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                    lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]),
                    source, flags=re.S)
    masked = re.sub(r'^[ \t]*#.*(?:\\\n.*)*', lambda m: ' ' * len(m[0]), masked, flags=re.M)
    depth = 0
    for match in re.finditer(r'[{}]', masked):
        if match[0] == '{':
            if depth == 0:
                end = match.start()
                pos = end - 1
                while pos >= 0 and masked[pos].isspace():
                    pos -= 1
                if pos >= 0 and masked[pos] == ')':
                    nesting = 1
                    pos -= 1
                    while pos >= 0 and nesting:
                        nesting += (masked[pos] == ')') - (masked[pos] == '(')
                        pos -= 1
                    while pos >= 0 and masked[pos].isspace():
                        pos -= 1
                    last = pos + 1
                    while pos >= 0 and (masked[pos].isalnum() or masked[pos] == '_'):
                        pos -= 1
                    name = masked[pos + 1:last]
                    if name:
                        yield name, match.end()
            depth += 1
        else:
            depth -= 1


def paths():
    selected = set(ROOT.glob('types/**/*.c')) | set(ROOT.glob('util/*.c')) | set(ROOT.glob('render/**/*.c'))
    selected |= set(ROOT.glob('backend/*.c'))
    for backend in ('drm', 'wayland', 'x11', 'multi', 'session'):
        selected |= set(ROOT.glob('backend/' + backend + '/*.c'))
    selected |= {ROOT / 'reference/dwl/dwl' / f for f in ('dwl.c', 'client.h', 'ime.h', 'util.h')}
    return sorted(selected)


def main():
    inventory = json.loads((ROOT / 'scripts/profiling-coverage.json').read_text())
    expected = {row['file']: row['functions'] for row in inventory}
    errors = []
    count = 0
    for path in paths():
        relative = str(path.relative_to(ROOT))
        source = path.read_text()
        actual = []
        for name, offset in functions(source):
            profiled = source[offset:].lstrip().startswith('WLR_PROFILE_SCOPE();')
            actual.append({'function': name, 'profiled': profiled})
            count += profiled
        if actual != expected.get(relative):
            errors.append(relative)
    if errors:
        print('Scope inventory differs: ' + ', '.join(errors), file=sys.stderr)
        return 1
    print(f'Checked {count} CPU scopes in {len(expected)} files; exclusions recorded in inventory.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
