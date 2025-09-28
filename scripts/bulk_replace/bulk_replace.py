#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# bulk_replace.py — mode recherche, ou cherche & remplace trouvera tous
#					les fichiers dans lesquels le 1er argument se retrouve,
# 					avec une copie des lignes suivantes et precedentes.
#
# Usage rapide
#   ./bulk_replace.py ARG_RECHERCHE NEW_ARG		# remplacement : OLD → NEW
#   ./bulk_replace.py ARG_RECHERCHE				# recherche : liste les occurrences
#
#
# Options principales
#   -h, --help                # affiche ce résumé
#   --path-uniq PATH          # dossier utilisé uniquement pour cette exécution
#   --path-base PATH          # modifie DEFAULT_PATH_BASE (valeur persistée)
#   --path-save SUFFIX        # mémorise un suffixe relatif à DEFAULT_PATH_BASE
#   --path-pwd                # affiche le chemin enregistré (base + suffixe)
#   --path-reset              # supprime le suffixe enregistré
#   --path-cd EXTRA           # ajoute un sous-chemin temporaire pour cet appel
#   --ignore NAME [...]       # noms de dossiers ignorés (.git, node_modules, ...)
#   --dry-run                 # prévisualisation sans écriture
#   --yes / --no              # applique tout / n’applique rien sans interaction
#   --no-interactive          # désactive les prompts interactifs
#   --show-all                # en mode recherche, liste immédiatement toutes les occurrences
#
#
# Points clés
# - DEFAULT_PATH_BASE (modifiez-le via --path-base) est stocké dans
#   ~/.config/bulk_replace/path.txt avec le suffixe éventuel (--path-save).
# - --path-reset efface uniquement le suffixe. --path-pwd affiche le chemin
#   complet (base + suffixe) ; --path-cd ajoute un complément temporaire.
# - patterns.txt doit se trouver dans le même dossier que ce script.
# - Mode recherche : donner uniquement QUERY (arg1). Sans option, parcours interactif;
#   utiliser --show-all pour afficher toutes les occurrences directement.
# - En mode recherche interactif : [Space]/[Enter]=marquer, [n]/[Backspace]=ignorer,
#   chiffres 1–9=changer le contexte affiché, 's'=ignorer le fichier courant,
#   'q'=skip all et afficher le récapitulatif.
# - Mode remplacement : fournir OLD (arg1) et NEW (arg2).
#   * Prompts interactifs : [Space]/[Enter]=appliquer, [n]/[Backspace]=ignorer,
#     chiffres 1–9=changer le contexte affiché, 'a'=appliquer le reste du fichier,
#     's'=ignorer le reste du fichier, 'q'=skip all.
#   * Chaque changement redessine le bloc au sommet du terminal pour garder le focus.
#   * Après validation, le bloc est re-affiché brièvement avec le contexte par défaut (2/2).
#
#
#
# Table des matieres reglages
#   L76  DEFAULT_PATH_BASE          Modifier la base par défaut avant tout enregistrement.
#   L189 save_config(...)           Changer le format du fichier ~/.config/bulk_replace/path.txt.
#   L271 collect_files(...)         Ajuster la découverte/filtrage des fichiers ciblés.
#   L343 render_block(...)          Adapter l'affichage interactif (messages, couleurs, contexte).
#   L371 print_header(...)          Personnaliser l'entête SEARCH/REPLACE (textes, ordre, infos).
#   L386 run_search_only(...)       Choisir ce qui est affiché pour chaque hit en mode recherche.
#   L548 main(...)                  Ajouter des options CLI ou modifier les comportements par défaut.
#
#


import argparse
import os
from pathlib import Path
import re
import sys
from typing import Optional, Tuple

# --- Enable ANSI on Windows if colorama is present ---
try:
    import colorama  # type: ignore
    colorama.just_fix_windows_console()
except Exception:
    pass

SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_PATTERNS_FILE = SCRIPT_DIR / "patterns.txt"

# Base path used when no override is supplied (edit to match your workspace).
DEFAULT_PATH_BASE = "~/Prog42/"
CONFIG_DIR = Path.home() / ".config" / "bulk_replace"
CONFIG_FILE = CONFIG_DIR / "path.txt"

ANSI_ESCAPE_RE = re.compile(r"\x1b\[[0-9;]*m")

# Text hooks that can be customized to inject extra output without touching the logic.
HEADER_PREFIX_TEXT = ""
BLOCK_PREFIX_TEXT = ""
BLOCK_SUFFIX_TEXT = ""
FINAL_SUFFIX_TEXT = ""


def normalize_suffix_input(raw: str) -> str:
    """Trim surrounding whitespace and slashes, keep interior structure."""
    trimmed = raw.strip()
    if not trimmed:
        return ""
    while trimmed.startswith('/'):
        trimmed = trimmed[1:]
    while trimmed.endswith('/'):
        trimmed = trimmed[:-1]
    return trimmed


def append_segment(path: Path, segment: str) -> Path:
    clean = normalize_suffix_input(segment)
    if not clean:
        return path
    for part in clean.split('/'):
        if part:
            path /= part
    return path


def build_effective_path(base: str, saved: Optional[str], extra: Optional[str]) -> Path:
    path = Path(base).expanduser()
    if saved:
        path = append_segment(path, saved)
    if extra:
        path = append_segment(path, extra)
    return path


def path_to_display(path: Path) -> str:
    expanded = path.expanduser()
    try:
        home = Path.home().expanduser()
        home_posix = home.as_posix()
    except Exception:
        home_posix = ''
    path_str = expanded.as_posix()
    if home_posix and path_str.startswith(home_posix):
        display = "~" + path_str[len(home_posix):]
        if display == "~":
            display = "~"
    else:
        display = path_str
    if not display.endswith('/'):
        display += '/'
    return display


def short_display_for_saved(path: Path, base: str, saved: Optional[str]) -> str:
    if saved:
        try:
            base_path = Path(base).expanduser()
            relative = path.relative_to(base_path)
            return f"~/.../{relative.as_posix()}"
        except Exception:
            pass
    display = path_to_display(path)
    if display.endswith('/'):
        display = display[:-1]
    return display


def visible_length(text: str) -> int:
    """Return printable width after stripping ANSI escape codes."""
    return len(ANSI_ESCAPE_RE.sub('', text))


def print_optional(text: str) -> int:
    """Print text if non-empty and return number of lines emitted."""
    if not text:
        return 0
    print(text)
    return text.count('\n') + 1


def build_relative_display(saved: Optional[str], extra: Optional[str]) -> str:
    segments = []
    if saved:
        clean_saved = normalize_suffix_input(saved)
        if clean_saved:
            segments.append(clean_saved)
    if extra:
        clean_extra = normalize_suffix_input(extra)
        if clean_extra:
            segments.append(clean_extra)
    if not segments:
        return "/"
    relative = "/" + "/".join(segments)
    if not relative.endswith('/'):
        relative += '/'
    return relative


def load_config() -> Tuple[Optional[str], Optional[str]]:
    if not CONFIG_FILE.exists():
        return None, None
    try:
        raw = CONFIG_FILE.read_text(encoding="utf-8")
    except Exception:
        return None, None
    base = None
    suffix = None
    for line in raw.splitlines():
        if not line.strip():
            continue
        key, _, value = line.partition(':')
        if not _:
            continue
        clean_value = value.lstrip('\t ').rstrip()
        if key.strip() == "DEFAULT_PATH_BASE":
            base = clean_value or None
        elif key.strip() == "--path-save":
            suffix = normalize_suffix_input(clean_value)
            if not suffix:
                suffix = None
    return base, suffix


# Update here if you want to persist extra keys alongside base/suffix.
def save_config(base: str, suffix: Optional[str]) -> None:
    CONFIG_DIR.mkdir(parents=True, exist_ok=True)
    base_value = base.strip()
    suffix_value = normalize_suffix_input(suffix) if suffix else ""
    content = (
        f"DEFAULT_PATH_BASE:\t{base_value}\n"
        f"--path-save:\t\t{suffix_value}\n"
    )
    CONFIG_FILE.write_text(content, encoding="utf-8")

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

def clear_after_header(static_lines: int) -> None:
    if static_lines < 0:
        static_lines = 0
    row = static_lines + 1
    sys.stdout.write(f"\033[{row};1H{ANSI['clear_to_end']}")
    sys.stdout.flush()

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

# Adjust iteration here to change which files are scanned (extensions, depth, etc.).
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
# Tweak prompts/context rendering here to change the interactive preview look & feel.
def render_block(file_label: str, line_num, total_lines, content_lines, context,
        before_col, after_col, mode_label, controls_hint,
        *, occurrence_progress: Optional[str] = None,
        file_progress: Optional[str] = None,
        inline_mode=True, static_lines: int = 0):
    """Imprime le bloc avec contexte autour de 'line_num' (1-based).
       Retourne le nombre de lignes imprimées (bloc + prompt)."""
    clear_after_header(static_lines)
    top = max(1, line_num - context)
    bottom = min(total_lines, line_num + context)
    printed = 0
    printed += print_optional(BLOCK_PREFIX_TEXT)
    if occurrence_progress:
        print(occurrence_progress); printed += 1
    if file_progress:
        print(file_progress); printed += 1
    file_line = f"\033[1mFile:\033[0m {file_label}"
    print(file_line); printed += 1
    divider_basis = file_line
    if mode_label:
        if inline_mode:
            divider_basis = mode_label
            print(mode_label)
        else:
            line_str = f"  {mode_label}"
            divider_basis = line_str
            print(line_str)
        printed += 1
    divider_width = max(visible_length(divider_basis), 1)
    print("-" * divider_width); printed += 1
    for i in range(top, bottom + 1):
        prefix = f"{i:>6d}: "
        if i == line_num:
            if before_col is not None:
                print(prefix + f"- {before_col}"); printed += 1
            if after_col is not None:
                print(prefix + f"+ {after_col}"); printed += 1
        else:
            print(prefix + content_lines[i-1].rstrip('\n')); printed += 1
    if controls_hint:
        print(controls_hint); printed += 1
    printed += print_optional(BLOCK_SUFFIX_TEXT)
    return printed

def print_header(resolved_path: str, suffix_display: str, patterns, ignore) -> int:
    lines = 0
    lines += print_optional(HEADER_PREFIX_TEXT)
    print(f"PATH: {suffix_display}"); lines += 1
    print(f"Resolved: {resolved_path}"); lines += 1
#    print(f"Patterns from: {DEFAULT_PATTERNS_FILE}"); lines += 1
#    print("Patterns:"); lines += 1
#    for pat in patterns:
#        print(f"  - {pat}"); lines += 1
#    print(f"Ignore dirs: {', '.join(ignore)}")
    print("------------------------------"); lines += 1
    return lines


# --- SEARCH mode ---
# Modify this to change search-mode reporting (sorting, extra context, colors).
def run_search_only(query: str, root: Path, patterns, ignore,
        show_all: bool, interactive: bool,
        resolved_path: str, suffix_display: str,
        base_value: str, saved_suffix: Optional[str]):
    files = collect_files(root, patterns, ignore)
    static_lines = 0
    print(""); static_lines += 1
    print("===== SEARCH MODE ====="); static_lines += 1
    static_lines += print_header(resolved_path, suffix_display, patterns, ignore)

    files_scanned = 0
    total_occurrences = 0

    if show_all or not interactive:
        for p in files:
            try:
                text = p.read_text(encoding="utf-8", errors="replace")
            except Exception as e:
                print(f"[WARN] Could not read {p}: {e}", file=sys.stderr)
                continue
            files_scanned += 1
            lines = text.splitlines(keepends=False)
            hits_here = []
            for i, line in enumerate(lines, start=1):
                if query in line:
                    total_occurrences += 1
                    hits_here.append((i, line))
            if hits_here:
                print(f"File: {p}")
                for (lineno, line) in hits_here:
                    print(f"  Line {lineno}: {colorize_all(line, query, ANSI['red']+ANSI['bold'])}")
                print("")
        print("------------------------------")
        print(f"Files scanned: {files_scanned}")
        print(f"Total occurrences: {total_occurrences}")
        if total_occurrences == 0:
            print("No matches found.")
        print_optional(FINAL_SUFFIX_TEXT)
        return 0

    marked_hits = []
    default_context = 2
    skip_all = False

    file_entries = []
    for p in files:
        try:
            text = p.read_text(encoding="utf-8", errors="replace")
        except Exception as e:
            print(f"[WARN] Could not read {p}: {e}", file=sys.stderr)
            continue
        files_scanned += 1
        lines = text.splitlines(keepends=True)
        occ_in_file = 0
        for raw_line in lines:
            occ_in_file += raw_line.rstrip('\n').count(query)
        if occ_in_file:
            file_entries.append({
                "path": p,
                "lines": lines,
                "occurrences": occ_in_file,
                "display": short_display_for_saved(p, base_value, saved_suffix),
            })

    total_occurrences_planned = sum(entry["occurrences"] for entry in file_entries)
    total_files_with_hits = len(file_entries)
    global_occ_index = 0

    search_controls_hint = "[Space/Enter]=mark  [n/Backspace]=skip  [1-9]=context  [s]=skip file  [q]=skip all"

    for file_idx, entry in enumerate(file_entries, start=1):
        if skip_all:
            break
        p = entry["path"]
        lines = entry["lines"]
        file_label = entry["display"]
        skip_file = False
        for idx, raw_line in enumerate(lines):
            line_no = idx + 1
            plain_line = raw_line.rstrip('\n')
            occ_in_line = plain_line.count(query)
            if occ_in_line == 0:
                continue
            occ_idx = 0
            while occ_idx < occ_in_line:
                total_occurrences += 1
                global_occ_index += 1
                local_context = default_context
                highlight_line = colorize_once(plain_line, query, ANSI["red"] + ANSI["bold"], occ_idx)
                mode_label = f"Line {line_no} (occurrence {occ_idx + 1}/{occ_in_line})  •  context={local_context}"
                occ_progress = (
                    f"Occ {global_occ_index}/{total_occurrences_planned}"
                    if total_occurrences_planned
                    else None
                )
                file_progress = (
                    f"File# {file_idx}/{total_files_with_hits}"
                    if total_files_with_hits
                    else None
                )

                render_block(
                    file_label,
                    line_no,
                    len(lines),
                    lines,
                    local_context,
                    highlight_line,
                    None,
                    mode_label,
                    search_controls_hint,
                    occurrence_progress=occ_progress,
                    file_progress=file_progress,
                    inline_mode=False,
                    static_lines=static_lines,
                )

                mark_this = False
                while True:
                    key = get_single_key()
                    if key in (' ', '\r', '\n'):
                        mark_this = True
                        clear_after_header(static_lines)
                        break
                    if key in ('\x08', '\x7f') or (key and key.lower() == 'n'):
                        mark_this = False
                        clear_after_header(static_lines)
                        break
                    if key and key.isdigit() and '1' <= key <= '9':
                        local_context = int(key)
                        clear_after_header(static_lines)
                        mode_label = f"Line {line_no} (occurrence {occ_idx + 1}/{occ_in_line})  •  context={local_context}"
                        highlight_line = colorize_once(plain_line, query, ANSI["red"] + ANSI["bold"], occ_idx)
                        render_block(
                            file_label,
                            line_no,
                            len(lines),
                            lines,
                            local_context,
                            highlight_line,
                            None,
                            mode_label,
                            search_controls_hint,
                            occurrence_progress=occ_progress,
                            file_progress=file_progress,
                            inline_mode=False,
                            static_lines=static_lines,
                        )
                        continue
                    if key and key.lower() == 's':
                        skip_file = True
                        clear_after_header(static_lines)
                        break
                    if key and key.lower() == 'q':
                        skip_all = True
                        skip_file = True
                        clear_after_header(static_lines)
                        break
                    if key == '':
                        clear_after_header(static_lines)
                        break
                    # Ignore other input and wait again
                if skip_all or skip_file:
                    break

                if mark_this:
                    start = max(1, line_no - local_context)
                    end = min(len(lines), line_no + local_context)
                    context_lines = []
                    for num in range(start, end + 1):
                        content = lines[num - 1].rstrip('\n')
                        if num == line_no:
                            content = colorize_once(content, query, ANSI["red"] + ANSI["bold"], occ_idx)
                        context_lines.append((num, content))
                    marked_hits.append({
                        "path": p,
                        "display": file_label,
                        "context": context_lines,
                    })

                occ_idx += 1
            if skip_all or skip_file:
                break
        if skip_all:
            break
    clear_after_header(static_lines)
    print("------------------------------")
    print(f"Files scanned: {files_scanned}")
    print(f"Occurrences inspected: {total_occurrences}")
    print(f"Occurrences marked: {len(marked_hits)}")

    if not marked_hits:
        print("No occurrences marked.")
        print_optional(FINAL_SUFFIX_TEXT)
        return 0

    print("\n===== MARKED OCCURRENCES =====")
    for entry in marked_hits:
        print(f"File: {entry['display']}")
        for num, content in entry["context"]:
            prefix = f"{num:>6d}: "
            print(prefix + content)
        print("")

    print_optional(FINAL_SUFFIX_TEXT)
    return 0

# Central CLI wiring: add/remove args or change defaults in this function.
def main():
    if sys.stdout.isatty():
        os.system("cls" if os.name == "nt" else "clear")
    parser = argparse.ArgumentParser(
        description="Find & replace using co-located patterns.txt. Single-arg SEARCH mode lists file/line locations.",
        epilog=f"Patterns file: {DEFAULT_PATTERNS_FILE}"
    )
    parser.add_argument("arg1", nargs="?", help="OLD (replace mode) or QUERY (search mode)")
    parser.add_argument("arg2", nargs="?", help="NEW (replace mode only)")
    parser.add_argument("--path-uniq", dest="path_uniq", help="Override working directory for this run only")
    parser.add_argument("--path-base", dest="path_base", help="Update DEFAULT_PATH_BASE (stored)")
    parser.add_argument("--path-save", dest="path_save", help="Persist relative suffix (without '~')")
    parser.add_argument("--path-pwd", action="store_true", help="Show DEFAULT_PATH_BASE combined with saved suffix")
    parser.add_argument("--path-reset", action="store_true", help="Clear the saved suffix")
    parser.add_argument("--path-cd", dest="path_cd", help="Temporary extra subpath for this run")
    parser.add_argument("--ignore", nargs="*", default=[".git", "node_modules", "build", "dist", ".idea", ".vscode", ".cache"], help="Directory names to ignore anywhere in the path")
    parser.add_argument("--dry-run", action="store_true", help="Preview only; do not write files (replace mode)")
    parser.add_argument("--yes", action="store_true", help="Apply all without prompting (replace mode)")
    parser.add_argument("--no", action="store_true", help="Skip all writes (replace mode)")
    parser.add_argument("--no-interactive", action="store_true", help="Disable interactive prompts (replace mode)")
    parser.add_argument("--show-all", action="store_true", help="SEARCH mode: print every occurrence without interactive review")
    args = parser.parse_args()

    stored_base, saved_suffix = load_config()
    base_value = stored_base if stored_base else DEFAULT_PATH_BASE
    performed_action = False

    if args.path_reset:
        saved_suffix = None
        save_config(base_value, saved_suffix)
        print("[INFO] Saved path cleared.")
        performed_action = True

    if args.path_base is not None:
        base_candidate = args.path_base.strip()
        if not base_candidate:
            raise SystemExit("[ERROR] --path-base expects a non-empty path.")
        base_value = base_candidate
        save_config(base_value, saved_suffix)
        base_display = path_to_display(Path(base_value).expanduser())
        print(f"[INFO] DEFAULT_PATH_BASE set to: {base_display}")
        performed_action = True

    if args.path_save is not None:
        raw_save = args.path_save.strip()
        if raw_save.startswith("~"):
            raise SystemExit("[ERROR] --path-save expects a relative suffix (no '~').")
        new_suffix = normalize_suffix_input(args.path_save)
        if new_suffix:
            saved_suffix = new_suffix
            save_config(base_value, saved_suffix)
            saved_display = path_to_display(build_effective_path(base_value, saved_suffix, None))
            relative_saved = build_relative_display(saved_suffix, None)
            print(f"[INFO] Saved path suffix: {relative_saved} (full: {saved_display})")
        else:
            saved_suffix = None
            save_config(base_value, saved_suffix)
            print("[INFO] Saved path cleared (empty suffix).")
        performed_action = True

    if args.path_pwd:
        current_display = path_to_display(build_effective_path(base_value, saved_suffix, None))
        print(current_display)
        performed_action = True

    cd_suffix = None
    if args.path_cd is not None:
        cd_candidate = normalize_suffix_input(args.path_cd)
        cd_suffix = cd_candidate or None

    if args.path_cd and args.arg1 is None:
        display_with_cd = path_to_display(build_effective_path(base_value, saved_suffix, cd_suffix))
        print(display_with_cd)
        return 0

    if args.arg1 is None:
        if performed_action:
            return 0
        parser.error("OLD/QUERY argument is required (see --help).")

    patterns = read_patterns_file(DEFAULT_PATTERNS_FILE)

    if args.path_uniq is not None:
        root_path = Path(args.path_uniq).expanduser()
        if cd_suffix:
            root_path = append_segment(root_path, cd_suffix)
        suffix_display = build_relative_display(None, cd_suffix)
    else:
        root_path = build_effective_path(base_value, saved_suffix, cd_suffix)
        suffix_display = build_relative_display(saved_suffix, cd_suffix)

    root_path = root_path.resolve()
    resolved_display = path_to_display(root_path)

    ignore_list = args.ignore
    search_interactive = sys.stdin.isatty() and (not args.show_all) and (not args.no_interactive)

    # SEARCH MODE (single argument)
    if args.arg2 is None:
        query = args.arg1
        return run_search_only(
            query,
            root_path,
            patterns,
            ignore_list,
            show_all=args.show_all,
            interactive=search_interactive,
            resolved_path=resolved_display,
            suffix_display=suffix_display,
            base_value=base_value,
            saved_suffix=saved_suffix,
        )

    # REPLACE MODE (two arguments: old -> new)
    old = args.arg1
    new = args.arg2
    if args.yes and args.no:
        raise SystemExit("[ERROR] --yes and --no are mutually exclusive.")

    candidates = collect_files(root_path, patterns, ignore_list)
    interactive_mode = sys.stdin.isatty() and (not args.no_interactive) and (not args.yes) and (not args.no) and (not args.dry_run)

    files_scanned = len(candidates)
    total_occ_previewed = 0
    total_occ_applied = 0
    write_reports = []

    static_lines = 0
    print(""); static_lines += 1
    print("===== REPLACE MODE ====="); static_lines += 1
    static_lines += print_header(resolved_display, suffix_display, patterns, ignore_list)
    print(f"Old → New: '{old}' → '{new}'"); static_lines += 1
#    print(f"Dry run: {args.dry_run}"); static_lines += 1
#    print(f"Interactive: {interactive_mode} (per occurrence with full-block redraw)"); static_lines += 1
    print("------------------------------"); static_lines += 1
    print(""); static_lines += 1

    default_context = 2

    file_entries = []
    for p in candidates:
        try:
            text = p.read_text(encoding="utf-8", errors="replace")
        except Exception as e:
            print(f"[WARN] Could not read {p}: {e}", file=sys.stderr)
            continue

        lines = text.splitlines(keepends=True)
        occ_in_file = 0
        for content_line in lines:
            occ_in_file += content_line.count(old)
        if occ_in_file:
            file_entries.append({
                "path": p,
                "lines": lines,
                "occurrences": occ_in_file,
            })

    total_occurrences_planned = sum(entry["occurrences"] for entry in file_entries)
    total_files_with_hits = len(file_entries)
    global_occ_index = 0
    skip_all = False
    replace_controls_hint = "[Space/Enter]=apply  [n/Backspace]=skip  [1-9]=context  [a]=apply rest(file)  [s]=skip rest(file)  [q]=skip all"

    for file_idx, entry in enumerate(file_entries, start=1):
        if skip_all:
            break
        p = entry["path"]
        lines = entry["lines"]
        file_occ_applied = 0
        skip_rest_file = False
        apply_rest_file = False
        file_label_display = str(p)

        for idx, line in enumerate(lines):
            occ = line.count(old)
            if occ == 0:
                continue

            occ_idx = 0
            while occ_idx < occ:
                prv = preview_once(line, old, new, occ_idx)
                if prv[0] is None:
                    break
                _, before_col, after_line, after_col = prv

                total_occ_previewed += 1
                global_occ_index += 1

                local_context = default_context
                mode_label = f"Line {idx+1} (occurrence {occ_idx+1}/{occ})  •  context={local_context}"
                occ_progress = (
                    f"Occ {global_occ_index}/{total_occurrences_planned}"
                    if total_occurrences_planned
                    else None
                )
                file_progress = (
                    f"File# {file_idx}/{total_files_with_hits}"
                    if total_files_with_hits
                    else None
                )

                render_block(
                    file_label_display,
                    idx + 1,
                    len(lines),
                    lines,
                    local_context,
                    before_col,
                    after_col,
                    mode_label,
                    replace_controls_hint,
                    occurrence_progress=occ_progress,
                    file_progress=file_progress,
                    static_lines=static_lines,
                )

                apply_this = False
                if args.dry_run or args.no:
                    clear_after_header(static_lines)
                    apply_this = False
                elif args.yes or args.no_interactive or apply_rest_file:
                    apply_this = True
                    clear_after_header(static_lines)
                elif skip_rest_file:
                    apply_this = False
                    clear_after_header(static_lines)
                elif interactive_mode:
                    while True:
                        key = get_single_key()
                        if key in (' ', '\r', '\n'):
                            apply_this = True
                            clear_after_header(static_lines)
                            break
                        elif key in ('\x08', '\x7f') or (key and key.lower() == 'n'):
                            apply_this = False
                            clear_after_header(static_lines)
                            break
                        elif key and key.isdigit() and '1' <= key <= '9':
                            local_context = int(key)
                            clear_after_header(static_lines)
                            mode_label = f"Line {idx+1} (occurrence {occ_idx+1}/{occ})  •  context={local_context}"
                            render_block(
                                file_label_display,
                                idx + 1,
                                len(lines),
                                lines,
                                local_context,
                                before_col,
                                after_col,
                                mode_label,
                                replace_controls_hint,
                                occurrence_progress=occ_progress,
                                file_progress=file_progress,
                                static_lines=static_lines,
                            )
                            continue
                        elif key and key.lower() == 'a':
                            apply_rest_file = True
                            apply_this = True
                            clear_after_header(static_lines)
                            break
                        elif key and key.lower() == 's':
                            skip_rest_file = True
                            apply_this = False
                            clear_after_header(static_lines)
                            break
                        elif key and key.lower() == 'q':
                            skip_all = True
                            skip_rest_file = True
                            apply_this = False
                            clear_after_header(static_lines)
                            occ_idx = occ
                            break
                        else:
                            continue

                if apply_this:
                    line = after_line
                    occ = line.count(old)
                    file_occ_applied += 1
                    total_occ_applied += 1
                    confirm_before = line.rstrip('\n')
                    confirm_after = colorize_all(confirm_before, new, ANSI["green"] + ANSI["bold"])
                    mode_label = f"Line {idx+1} (APPLIED)  •  context={default_context}"
                    render_block(
                        file_label_display,
                        idx + 1,
                        len(lines),
                        lines,
                        default_context,
                        confirm_before,
                        confirm_after,
                        mode_label,
                        "",
                        occurrence_progress=occ_progress,
                        file_progress=file_progress,
                        static_lines=static_lines,
                    )
                    clear_after_header(static_lines)

                lines[idx] = line
                occ_idx += 1

            if skip_all or skip_rest_file:
                break

        if not args.dry_run and not args.no and file_occ_applied > 0:
            try:
                p.write_text("".join(lines), encoding="utf-8")
                write_reports.append(f"→ Écrit: {p}  (occurrences appliquées: {file_occ_applied})")
            except Exception as e:
                print(f"[ERROR] Could not write {p}: {e}", file=sys.stderr)

    clear_after_header(static_lines)
    print("------------------------------")
    print(f"Files scanned:        {files_scanned}")
    print(f"Occurrences previewed:{total_occ_previewed}")
    print(f"Occurrences applied:  {total_occ_applied}")
    if args.dry_run or args.no:
        print("(no files were written)")
    for report in write_reports:
        print(report)
    print_optional(FINAL_SUFFIX_TEXT)
    return 0

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        print("\nInterrupted.", file=sys.stderr)
        raise SystemExit(130)
