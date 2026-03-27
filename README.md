*This project has been created as part of the 42 curriculum by aylee, seoykim.*

# minishell

## Description

minishell is a project from the 42 curriculum that involves building a simple Unix shell, inspired by Bash. The goal is to gain deep understanding of processes, file descriptors, signals, and how a shell parses and executes commands.

The shell supports interactive prompt display, command history, pipes, redirections, environment variable expansion, and a set of built-in commands — all while managing memory carefully and handling signals correctly.

## Instructions

### Requirements

- Linux or macOS
- `gcc` with `-Wall -Wextra -Werror`
- `readline` library installed

### Compilation

```bash
make
```

This will compile the project and produce the `minishell` executable.

### Running

```bash
./minishell
```

You will be greeted with a prompt:

```
minishell$
```

### Makefile rules

| Rule | Description |
|------|-------------|
| `make` / `make all` | Compile the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and binary |
| `make re` | Full recompile |

### Memory leak check (Valgrind)

`readline` has known internal memory leaks that are not caused by our code. Use the provided suppression file to filter them out:

```bash
valgrind --suppressions=readline.supp ./minishell
```

## Features

- Interactive prompt with command history (arrow keys supported)
- Execute commands via `PATH`, relative path, or absolute path
- Pipes: `cmd1 | cmd2 | cmd3`
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Environment variable expansion (`$VAR`, `$?`)
- Single quotes `'` — prevent all interpretation
- Double quotes `"` — prevent interpretation except `$`
- Signal handling: `ctrl-C`, `ctrl-D`, `ctrl-\`
- Built-in commands:
  - `echo [-n]`
  - `cd <path>`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## Resources

### References

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Linux man pages: fork, execve, pipe, dup2, waitpid](https://man7.org/linux/man-pages/index.html)
- [Writing Your Own Shell — Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [The Linux Programming Interface — Michael Kerrisk](https://man7.org/tlpi/)

### AI Usage

AI (Claude) was used in this project for the following purposes:

- Debugging memory leaks identified by Valgrind — AI helped analyze stack traces and identify which allocations were not being freed and at what point in the code cleanup should occur.
- Reformatting code to comply with 42 Norm rules (e.g., splitting functions that exceeded the 25-line limit).
- Clarifying whether certain Valgrind `still reachable` entries originated from system binaries (`/usr/bin/grep`, `/usr/bin/ls`) rather than our own code.

All AI-generated suggestions were reviewed, tested, and fully understood before being incorporated into the project.
