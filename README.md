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
   - The **expander** rewrites tokens (quotes, `$?`, `$VAR`, heredoc policy) and
     keeps the working set inside the current `t_pool` context.
   - The **parser** consumes the token list to build a linked list of `t_cmd`
     nodes (arguments, redirections, heredoc descriptors, builtin metadata).
   - Each command node carries the file descriptors that must survive until the
     executor stage.

4. **Execution**
   - Single builtin → run in the parent, save/restore stdio, keep the original
     process alive.
   - Pipelines → `execute.c` forks per node, wires pipes and redirections,
     delegates builtins or external binaries, tracks pids, and reconciles
     statuses once every child terminates.
   - On exit, the loop publishes the resulting `last_status` so the next prompt
     sees `$?` aligned with Bash semantics.

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

### t_pool Playbook

- Always check the return value of `pool_alloc_ctx`, `pool_track_ctx`,
  `pool_track_fd`, and `pool_open_ctx`. On failure, free the raw pointer if it
  is not tracked yet and bubble the error up (usually via `error_exit`).
- Register transient buffers (readline results, ft_split tables, heredoc lines)
  as soon as they are created so that abrupt exits cleanly release them.
- When replacing tracked pointers, assign the new value **before** freeing the
  old one to avoid losing references.
- Prefer the `_ctx` helpers over naked `malloc`/`open`; they keep the tracking
  lists consistent across forks and signal handlers.
- Use `pool_cleanup_ctx` in every fatal branch so both FDs and allocations are
  reclaimed before the process terminates.

### DEBUG instrumentation

Define `DEBUG_POOL` at build time (e.g. `make DEBUG_POOL=1`) to enable lightweight
logging whenever a pool allocation or tracking call fails. Messages are emitted
with `ft_putstr_fd` so they respect the 42 Norm and stay TTY-friendly.

## Error Handling & Signals

- `error_exit(pool, env, msg)` centralises fatal errors: it prints a prefixed
  message, calls `pool_cleanup_ctx`, and exits with status `1`.
- Non-fatal errors must set `exp->last_status` appropriately so `$?` mirrors the
  observed behaviour (127 for missing binaries, 126 on execve errors, heredoc
  signals, etc.).
- Syntax issues short-circuit the parser, discard partially built commands, and
  keep the shell loop alive with a non-zero status.
- Interactive signals mimic Bash: `SIGINT` during prompt redraws the line,
  `SIGQUIT` is ignored in interactive mode, and heredoc children restore
  default handlers so `Ctrl-C` aborts the delimiter capture.
- Always close tracked descriptors via `pool_close_ctx` once they are no longer
  needed to avoid leaking resources across forks.

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
