# Minishell Overview

Minishell is a lightweight command interpreter inspired by Bash. The program
parses the user input, expands variables, builds a pipeline of commands,
executes builtins or external programs, and keeps the process state consistent
with shell conventions (signals, exit codes, environment).

## Global Execution Flow

1. **Bootstrap** (`main.c`)
   - Initialise the global memory/FD pool and environment copies.
   - Install the interactive signal handlers.
   - Loop on `readline()` to fetch user input, add history, and dispatch it to
     `process_input`.

2. **Pre-processing**
   - Detect isolated assignments (`is_simple_assignment`) to update the local
     environment without launching a command.
   - Tokenise the line with the **lexer**.
   - Reject syntax errors early (pipes, redirections, unmatched quotes).

3. **Semantic Phases**
   - Expand variables and special sequences (handled by the **expander**).
   - Build a linked list of `t_cmd` structures with the **parser**.
   - Prepare heredocs when requested, wiring temporary pipes for later use.

4. **Execution**
   - If a single builtin is present, run it in the parent process to preserve the
     shell state.
   - Otherwise, `execute.c` forks for each pipeline stage, sets redirections,
     runs builtins or `execve`, collects exit codes, and restores terminal
     signals.
   - The shell loop resumes with the updated `last_status`.

## Module Breakdown

| Module        | Responsibility |
| ------------- | -------------- |
| `srcs/lexer`      | Tokenises the raw line (words, pipes, redirections, heredoc markers, quotes). |
| `srcs/parser`     | Converts tokens into `t_cmd` nodes, records redirections/heredocs, validates grammar. |
| `srcs/expander`   | Expands `$VAR`, `$?`, and performs quote-aware string assembly. |
| `srcs/execution`  | Manages pipelines, forks, redirections, builtins, path lookup, and exit statuses. |
| `srcs/builtin`    | Implements `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`. |
| `srcs/env`        | Maintains exported and local environments, identifier validation, `set_env` helpers. |
| `srcs/mem_manager`| Provides a pool to track allocations and FDs, simplifying cleanup on errors/exits. |
| `srcs/signals`    | Defines interactive signal handlers (`SIGINT`, `SIGQUIT`, heredoc-specific handler). |
| `srcs/utils`      | Miscellaneous helpers (error reporting, parent cleanup, string joins). |
| `libft`           | Custom libft used by the shell (string, list, and utility routines). |

## Memory and FD Management

The project relies on a custom pool (`t_pool`) to register every allocation or
file descriptor that must be released before the shell exits or before forking.
Key helpers include:

- `pool_alloc_ctx`, `pool_strdup_ctx`, `pool_strjoin_ctx` for allocations.
- `pool_track_ctx`, `pool_free_ctx`, `pool_cleanup_ctx` for precise cleanup.
- `pool_open_ctx`, `pool_open_coe_ctx`, `pool_close_ctx`, `pool_close_all_ctx`
  for descriptor tracking and `FD_CLOEXEC` handling.

This approach prevents leaks even when errors occur deep in the parsing or
execution stages.

## Error Handling & Signals

- Syntax errors prevent the parser from running and set `last_status` to a
  non-zero value.
- Execution errors print a descriptive message and propagate the failing status
  (`$?`).
- Interactive signals mirror Bash behaviour: `Ctrl-C` refreshes the prompt,
  `Ctrl-D` exits, `Ctrl-\` is ignored, and heredocs react to `SIGINT` by aborting
  the current delimiter capture.

## Building and Running

```bash
$ make        # builds minishell
$ ./minishell # launches the interactive shell
```

`readline` may leak internal buffers; this is accepted by the subject, provided
minishell’s own allocations are released.

## Useful Documents

- `minishell.pdf` — official 42 subject.
- `README Alberto.md` — legacy project notes.
- `srcs/bugs.md` — list of known issues awaiting fixes.

Feel free to enrich this README with additional usage notes, debugging tips, or
contributor guidelines as the project evolves.
