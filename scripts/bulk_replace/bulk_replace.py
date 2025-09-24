#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# bulk_replace.py — find & replace with co-located patterns.txt,
# interactive per-occurrence confirms with full-block redraw (ancré en haut),
# and SEARCH mode.
#
# Points clés :
# - patterns.txt doit se trouver dans le même dossier que ce script.
# - Mode RECHERCHE (1 seul argument) : --show pour afficher le contenu des lignes.
# - Mode REMPLACEMENT (2 arguments) :
#     * Interactif par occurrence : [Space]/[Enter]=appliquer, [Backspace]/[Delete]=ignorer
#       [1–9]=changer le contexte (nb de lignes au-dessus & en dessous), [a]/[s]/[q] raccourcis.
#     * Redraw complet du bloc à chaque changement de contexte (pas d’empilement).
#     * Après acceptation, affiche brièvement avec contexte par défaut (2/2), puis continue.
#
import argparse
import os
from pathlib import Path
import sys

# --- Enable ANSI on Windows if colorama is present ---
try:
    import colorama  # type: ignore
    colorama.just_fix_windows_console()
except Exception:
    pass

SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_PATTERNS_FILE = SCRIPT_DIR / "patterns.txt"

# --- Cross-platform single keypress ---
def get_single_key():
    """Return a single keypress as a string ('' if unsupported).
    Space=' ', Backspace='\x08' or '\x7f', Enter='\r' or '\n'."""
    try:
        # Windows
        import msvcrt  # type: ignore
    #    ch = msvcrt.getch()
    #    try:
    #        return ch.decode('utf-8', errors='ignore')
    #    except Exception:
    #        return ''
    except Exception:
        # POSIX
        try:
            import tty, termios  # type: ignore
            fd = sys.stdin.fileno()
            old_settings = termios.tcgetattr(fd)
            try:
                tty.setraw(fd)
                ch = sys.stdin.read(1)
                return ch
            finally:
                termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        except Exception:
            return ''

# ANSI helpers
ANSI = {
    "red": "\033[31m",
    "green": "\033[32m",
    "bold": "\033[1m",
    "reset": "\033[0m",
    "clear_line": "\033[2K",
    "clear_to_end": "\033[J",
}

def move_up(n: int) -> str:
    if n <= 0:
        return ''
    return f"\033[{n}A"

def clear_block(lines_count: int) -> str:
    """Revenir en haut du bloc et effacer jusqu’à la fin de l’écran (clean slate)."""
    if lines_count <= 0:
        return ''
    return "\r" + move_up(lines_count) + ANSI['clear_to_end']

def read_patterns_file(pfile: Path):
    if not pfile.exists():
        raise SystemExit(f"[ERROR] patterns.txt introuvable: {pfile}")
    try:
        content = pfile.read_text(encoding="utf-8")
    except Exception as e:
        raise SystemExit(f"[ERROR] Impossible de lire '{pfile}': {e}")
    patterns = []
    for raw in content.splitlines():
        line = raw.strip()
        if not line or line.startswith('#'):
            continue
        patterns.append(line)
    if not patterns:
        raise SystemExit("[ERROR] patterns.txt est vide (après filtres).")
    return patterns

def is_under_ignored_dir(path: Path, ignore_names):
    for part in path.parts:
        if part in ignore_names:
            return True
    return False

def collect_files(root: Path, patterns, ignore_names):
    found = []
    for pat in patterns:
        for p in root.rglob(pat):
            if p.is_file() and not is_under_ignored_dir(p.relative_to(root), set(ignore_names)):
                found.append(p)
    # de-dup (order-preserving)
    seen = set()
    unique = []
    for p in found:
        if p not in seen:
            unique.append(p)
            seen.add(p)
    return unique

# --- line coloring helpers ---
def colorize_all(line: str, needle: str, color_code: str):
    if not needle:
        return line
    parts = line.split(needle)
    if len(parts) == 1:
        return line
    out = []
    for i, part in enumerate(parts):
        out.append(part)
        if i != len(parts) - 1:
            out.append(f"{color_code}{needle}{ANSI['reset']}")
    return "".join(out)

def colorize_once(line: str, needle: str, color_code: str, nth: int):
    if not needle:
        return line
    start = 0
    count = 0
    parts = []
    while True:
        idx = line.find(needle, start)
        if idx == -1:
            parts.append(line[start:])
            break
        parts.append(line[start:idx])
        frag = needle
        if count == nth:
            frag = f"{color_code}{needle}{ANSI['reset']}"
        parts.append(frag)
        start = idx + len(needle)
        count += 1
    return "".join(parts)

def preview_once(before: str, old: str, new: str, nth: int):
    # Locate nth occurrence
    idx = -1
    start = 0
    count = 0
    while True:
        i = before.find(old, start)
        if i == -1:
            return None, None, None, None
        if count == nth:
            idx = i
            break
        start = i + len(old)
        count += 1
    after = before[:idx] + new + before[idx+len(old):]
    before_col = colorize_once(before.rstrip('\n'), old, ANSI["red"] + ANSI["bold"], nth)
    after_col = after.rstrip('\n')
    after_col = after_col[:idx] + f"{ANSI['green']}{ANSI['bold']}{new}{ANSI['reset']}" + after_col[idx+len(new):]
    return idx, before_col, after, after_col

# --- context rendering ---
def render_block(file_path, line_num, total_lines, content_lines, context, before_col, after_col, mode_label):
    """Imprime le bloc avec contexte autour de 'line_num' (1-based).
       Retourne le nombre de lignes imprimées (bloc + prompt)."""
    top = max(1, line_num - context)
    bottom = min(total_lines, line_num + context)
    printed = 0
    print(f"\033[1mFile:\033[0m {file_path}  •  {mode_label}"); printed += 1
    for i in range(top, bottom + 1):
        prefix = f"{i:>6d}: "
        if i == line_num:
            print(prefix + f"- {before_col}"); printed += 1
            print(prefix + f"+ {after_col}"); printed += 1
        else:
            print(prefix + content_lines[i-1].rstrip('\n')); printed += 1
    print("[Space/Enter]=apply  [Backspace/Delete]=skip  [1-9]=context  [a]=apply rest(file)  [s]=skip rest(file)  [q]=next file"); printed += 1
    return printed

def print_header(root, patterns, ignore):
    print(f"Root: {root}")
    print("Patterns from:", DEFAULT_PATTERNS_FILE)
    print("Patterns:")
    for pat in patterns:
        print(f"  - {pat}")
    print(f"Ignore dirs: {', '.join(ignore)}")
    print("------------------------------")

# --- SEARCH mode ---
def run_search_only(query: str, root: Path, patterns, ignore, show):
    files = collect_files(root, patterns, ignore)
    total_hits = 0
    print("\n===== SEARCH MODE =====")
    print_header(root, patterns, ignore)
    for p in files:
        try:
            text = p.read_text(encoding="utf-8", errors="replace")
        except Exception as e:
            print(f"[WARN] Could not read {p}: {e}", file=sys.stderr)
            continue
        lines = text.splitlines(keepends=False)
        hits_here = []
        for i, line in enumerate(lines, start=1):
            if query in line:
                hits_here.append((i, line))
        if hits_here:
            print(f"File: {p}")
            for (lineno, line) in hits_here:
                total_hits += 1
                if show:
                    print(f"  Line {lineno}: {colorize_all(line, query, ANSI['red']+ANSI['bold'])}")
                else:
                    print(f"  Line {lineno}")
            print("")
    print("------------------------------")
    print(f"Files scanned: {len(files)}")
    print(f"Total occurrences: {total_hits}")
    if total_hits == 0:
        print("No matches found.")
    return 0

def main():
    parser = argparse.ArgumentParser(
        description="Find & replace using co-located patterns.txt. Single-arg SEARCH mode lists file/line locations."
    )
    parser.add_argument("arg1", help="OLD (replace mode) or QUERY (search mode)")
    parser.add_argument("arg2", nargs="?", help="NEW (replace mode only)")
    parser.add_argument("--root", default=".", help="Root directory to start from (default: current directory)")
    parser.add_argument("--ignore", nargs="*", default=[".git", "node_modules", "build", "dist", ".idea", ".vscode", ".cache"], help="Directory names to ignore anywhere in the path")
    parser.add_argument("--dry-run", action="store_true", help="Preview only; do not write files (replace mode)")
    parser.add_argument("--yes", action="store_true", help="Apply all without prompting (replace mode)")
    parser.add_argument("--no", action="store_true", help="Skip all writes (replace mode)")
    parser.add_argument("--no-interactive", action="store_true", help="Disable interactive prompts (replace mode)")
    parser.add_argument("--show", action="store_true", help="SEARCH mode: also print line contents with highlights")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    patterns = read_patterns_file(DEFAULT_PATTERNS_FILE)

    # SEARCH MODE (single argument)
    if args.arg2 is None:
        query = args.arg1
        return run_search_only(query, root, patterns, args.ignore, show=args.show)

    # REPLACE MODE (two arguments: old -> new)
    old = args.arg1
    new = args.arg2
    if args.yes and args.no:
        raise SystemExit("[ERROR] --yes and --no are mutually exclusive.")

    candidates = collect_files(root, patterns, args.ignore)
    interactive_mode = sys.stdin.isatty() and (not args.no_interactive) and (not args.yes) and (not args.no) and (not args.dry_run)

    files_scanned = len(candidates)
    total_occ_previewed = 0
    total_occ_applied = 0

    print("\n===== REPLACE MODE =====")
    print_header(root, patterns, args.ignore)
    print(f"Old → New: '{old}' → '{new}'")
    print(f"Dry run: {args.dry_run}")
    print(f"Interactive: {interactive_mode} (per occurrence with full-block redraw)")
    print("------------------------------\n")

    default_context = 2

    for p in candidates:
        try:
            text = p.read_text(encoding="utf-8", errors="replace")
        except Exception as e:
            print(f"[WARN] Could not read {p}: {e}", file=sys.stderr)
            continue

        lines = text.splitlines(keepends=True)
        file_occ_applied = 0
        skip_rest_file = False
        apply_rest_file = False

        for idx, line in enumerate(lines):
            occ = line.count(old)
            if occ == 0:
                continue

            occ_idx = 0
            while occ_idx < occ:
                prv = preview_once(line, old, new, occ_idx)
                if prv[0] is None:
                    break
                hit_pos, before_col, after_line, after_col = prv

                total_occ_previewed += 1

                # Live interactive preview with context and full-block redraw
                local_context = default_context
                mode_label = f"Line {idx+1} (occurrence {occ_idx+1}/{occ})  •  context={local_context}"
                block_lines = render_block(p, idx+1, len(lines), lines, local_context, before_col, after_col, mode_label)

                apply_this = False
                if args.dry_run or args.no:
                    # Clean and skip
                    sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                    apply_this = False
                elif args.yes or args.no_interactive or apply_rest_file:
                    apply_this = True
                    sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                elif skip_rest_file:
                    apply_this = False
                    sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                elif interactive_mode:
                    while True:
                        key = get_single_key()
                        if key in (' ', '\r', '\n'):  # apply
                            apply_this = True
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            break
                        elif key in ('\x08', '\x7f'):  # skip
                            apply_this = False
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            break
                        elif key and key.isdigit() and '1' <= key <= '9':
                            # Redraw whole block anchored at top with new context
                            local_context = int(key)
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            mode_label = f"Line {idx+1} (occurrence {occ_idx+1}/{occ})  •  context={local_context}"
                            block_lines = render_block(p, idx+1, len(lines), lines, local_context, before_col, after_col, mode_label)
                            continue
                        elif key and key.lower() == 'a':
                            apply_rest_file = True
                            apply_this = True
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            break
                        elif key and key.lower() == 's':
                            skip_rest_file = True
                            apply_this = False
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            break
                        elif key and key.lower() == 'q':
                            skip_rest_file = True
                            apply_this = False
                            sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()
                            occ_idx = occ  # jump to next file
                            break
                        else:
                            continue

                if apply_this:
                    # Apply single occurrence to the in-memory line
                    line = after_line
                    occ = line.count(old)  # recalc remaining after change
                    file_occ_applied += 1
                    total_occ_applied += 1
                    # Optional quick confirmation (render then clear) with default context
                    confirm_before = line.rstrip('\n')
                    confirm_after = colorize_all(confirm_before, new, ANSI["green"] + ANSI["bold"])
                    mode_label = f"Line {idx+1} (APPLIED)  •  context={default_context}"
                    block_lines = render_block(p, idx+1, len(lines), lines, default_context, confirm_before, confirm_after, mode_label)
                    sys.stdout.write(clear_block(block_lines)); sys.stdout.flush()

                # Update stored line
                lines[idx] = line
                occ_idx += 1

        if not args.dry_run and not args.no and file_occ_applied > 0:
            try:
                p.write_text("".join(lines), encoding="utf-8")
                print(f"→ Écrit: {p}  (occurrences appliquées: {file_occ_applied})")
            except Exception as e:
                print(f"[ERROR] Could not write {p}: {e}", file=sys.stderr)

    print("------------------------------")
    print(f"Files scanned:        {files_scanned}")
    print(f"Occurrences previewed:{total_occ_previewed}")
    print(f"Occurrences applied:  {total_occ_applied}")
    if args.dry_run or args.no:
        print("(no files were written)")
    return 0

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        print("\nInterrupted.", file=sys.stderr)
        raise SystemExit(130)
