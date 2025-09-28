#!/usr/bin/env python3
"""Vérifie les appels de fonctions des fichiers .c selon la liste du sujet.

Utilisation
-----------
    ./ctrl_fns_autorisees.py [--root chemin]
    
    
    
Modifs a apporter
-----------------
	utilisable partout
    recherche dans le dossier et  sous dossiers actuels
    selon liste des fonctions autorisees en .txt

    


La liste des fonctions autorisées est lue dans ``list_fns_py.txt`` (copier/coller
direct du sujet, séparateurs virgules ou retours à la ligne acceptés).

Constantes
----------
``KEYWORDS`` : mots-clés C ignorés (``if``, ``for``...) afin de ne pas les
               confondre avec des appels. for a ete enleve pour ressortir comme
               erreur si present.
               
``LOCAL_PREFIXES`` : préfixes de nos fonctions internes à ignorer (``ft_``,
                     ``pool_``...).

En retirant ``for`` de ``KEYWORDS``, le script le signalerait comme fonction
non autorisée. Même idée pour les préfixes : enlever ``ft_`` relèverait tout
libft.
"""

from __future__ import annotations
import argparse
import re
from pathlib import Path

FUNC_DEF_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_\s\*]*\b([A-Za-z_][A-Za-z0-9_]*)\s*\([^;]*\)\s*\{",
                         re.MULTILINE)
CALL_RE = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)\s*\(")
KEYWORDS = {"if", "while", "switch", "return", "sizeof"}
LOCAL_PREFIXES = (
    "ft_", "pool_", "lexer", "parser", "execute", "expand", "handle",
    "create", "add_", "free_", "set_", "get_", "init", "count", "take",
    "only", "cleanup", "print", "join", "find_", "remove", "unset",
    "is_", "process", "update", "safe_", "track", "check", "setup",
    "sig", "heredoc", "error_", "str_", "append_", "exp_", "env",
    "path", "initialise", "cleanup_", "join_", "find", "take_", "setup_",
    "unset_", "process_", "remove_", "cmd", "token",
)
LIST_PATH = Path(__file__).resolve().parent / "list_fns_py.txt"


def load_allowed() -> set[str]:
    if not LIST_PATH.exists():
        raise FileNotFoundError(f"Liste introuvable : {LIST_PATH}")
    raw = LIST_PATH.read_text()
    tokens = re.split(r"[\s,]+", raw)
    return {tok for tok in (t.strip() for t in tokens) if tok}


def collect_defined_functions(root: Path) -> set[str]:
    defined: set[str] = set()
    for cfile in root.rglob("*.c"):
        text = cfile.read_text()
        for match in FUNC_DEF_RE.finditer(text):
            defined.add(match.group(1))
    return defined


def collect_calls(root: Path) -> dict[str, list[tuple[Path, int]]]:
    calls: dict[str, list[tuple[Path, int]]] = {}
    for cfile in root.rglob("*.c"):
        try:
            if Path("libft") in cfile.relative_to(root).parents:
                continue
        except ValueError:
            # cfile not inside root (should not happen)
            pass
        with cfile.open() as fh:
            for lineno, line in enumerate(fh, 1):
                stripped = line.strip()
                if stripped.startswith(("//", "/*", "*", "*/")):
                    continue
                for match in CALL_RE.finditer(line):
                    name = match.group(1)
                    if name.isupper():
                        continue
                    before = line[:match.start()].rstrip()
                    if before.endswith('.') or before.endswith('->'):
                        continue
                    if name in KEYWORDS:
                        continue
                    if match.start() > 0 and line[match.start() - 1] in {'"', "'", '%'}:
                        continue
                    calls.setdefault(name, []).append((cfile, lineno))
    return calls


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Report non-allowed function calls in .c files"
    )
    parser.add_argument(
        "--root",
        default=Path(__file__).resolve().parent.parent,
        help="project root (default: dossier parent)",
    )
    args = parser.parse_args()

    root = Path(args.root).resolve()
    allowed = load_allowed()
    defined = collect_defined_functions(root)
    calls = collect_calls(root)

    suspicious: dict[str, list[tuple[Path, int]]] = {}
    for func, locs in calls.items():
        if func in allowed or func in defined:
            continue
        if func.startswith(LOCAL_PREFIXES):
            continue
        suspicious[func] = locs

    if not suspicious:
        print("No unauthorized functions detected.")
        return 0

    print("Unauthorized function calls found:")
    for func in sorted(suspicious):
        print(f"- {func}")
        for path, lineno in suspicious[func]:
            rel = path.relative_to(root)
            print(f"  {rel}:{lineno}")
    return 1


if __name__ == "__main__":
    import sys
    try:
        sys.exit(main())
    except BrokenPipeError:
        pass
