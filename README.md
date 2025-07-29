# USH - Utkarsh's Shell

A minimalist UNIX shell implementation that proves you don't need 10,000 lines of code to get things done. Built with the philosophy of "keep it simple, make it work."

## Features

### What USH Can Do

* *   **Execute system commands** - Run any program available in your PATH
* *   **Built-in commands** - Essential shell builtins for core functionality
* *   **Command chaining** - Chain commands with `&&` (AND) and `||` (OR) operators
* *   **Memory management** - Dynamic buffer allocation that grows as needed
* *   **Error handling** - Proper error messages and graceful failure handling

### Built-in Commands

* *   `cd <directory>` - Change the current working directory
* *   `help` - Display available built-in commands and usage info
* *   `exit` - Exit the shell gracefully

### Command Chaining Examples

```bash
ush> ls -la && echo "Directory listed successfully"
ush> make || echo "Build failed, but life goes on"  
ush> cd /tmp && ls && echo "We're in tmp now"
```

## Building and Running

### Prerequisites

* *   GCC or any C compiler
* *   UNIX-like operating system (Linux, macOS, etc.)
* *   Basic POSIX libraries

### Compilation

```bash
gcc -o ush main.c
```

### Running

```bash
./ush
```

You'll see the friendly prompt:

```
Starting USH - Utkarsh's Shell
ush> 
```

## Architecture

USH follows a clean, modular design:

* *   **Input Reading** - Dynamic buffer allocation for command input
* *   **Tokenization** - Smart parsing of command line arguments
* *   **Command Execution** - Fork/exec model for running programs
* *   **Built-in Handler** - Separate system for internal commands
* *   **Logic Operations** - Clean handling of `&&` and `||` chaining

## Current Limitations

USH is intentionally minimal but honest about what it doesn't do (yet):

* *   No background job support (`&`)
* *   No signal handling (Ctrl+C, Ctrl+Z)
* *   No job control or tracking
* *   No pipes (`|`) or redirection (`>`, `<`)
* *   No command history or tab completion
* *   No environment variable expansion

## Code Stats

* *   **~250 lines** of clean, readable C code
* *   **Zero external dependencies** beyond standard libraries
* *   **Memory-safe** with proper allocation and cleanup
* *   **Error-resistant** with comprehensive error checking

## Design Philosophy

USH embodies the UNIX philosophy:

* *   Do one thing and do it well
* *   Keep it simple, stupid (KISS)
* *   Write programs that work together
* *   Favor clarity over cleverness

## Contributing

Found a bug? Want to add a feature? The codebase is small enough to understand in 10 minutes and clean enough to modify with confidence.

## Author

**Utkarsh Dabral** - July 29, 2025

Inspired by Stephen Brennan's LSH tutorial, but with enough personal touches to call it my own.

## License

This project is in the public domain. Use it, abuse it, learn from it, or laugh at it - your choice.

* * *

_"A shell so simple, even the documentation is under 100 lines."_
