# CLI Interpreter

A command-line interpreter implemented in **C++17** that simulates a subset of common shell commands. The project focuses on command parsing, modular command execution, file operations, input/output redirection, and command pipelines.

## Overview

The interpreter provides an interactive command-line environment where users can execute built-in commands, process text, manipulate files, and combine commands into pipelines.

Commands are parsed by a custom **lexer** and dispatched to their corresponding implementations. The project follows an object-oriented design, with a shared command interface that allows individual commands to be implemented and extended independently.

## Features

- **Interactive CLI** — execute commands through a custom command prompt.
- **Custom lexical analysis** — tokenize command input and handle quoted arguments.
- **Command pipelines** — connect commands using the `|` operator and pass output between consecutive commands.
- **Input and output handling** — read from standard input or files and redirect command output to files.
- **Output redirection** — support for overwriting files with `>` and appending output with `>>` in implemented commands.
- **Batch execution** — execute a sequence of commands from a text file.
- **File management** — create, truncate, and remove files.
- **Text processing** — count words or characters, extract lines, and replace text.
- **Error handling** — custom exception types for lexical errors, unknown commands, invalid syntax, and file operation failures.

## Supported Commands

| Command | Description |
| --- | --- |
| `echo` | Display text, read input, and write content to a file. |
| `prompt` | Customize the command-line prompt. |
| `time` | Display the current system time. |
| `date` | Display the current date. |
| `touch` | Create a new file. |
| `truncate` | Clear the contents of an existing file. |
| `rm` | Remove a file. |
| `wc` | Count words (`-w`) or characters (`-c`). |
| `tr` | Replace occurrences of a specified string. |
| `head` | Display the first *N* lines of input. |
| `batch` | Execute commands from a script file. |

The interpreter also supports `exit` and `quit` to terminate the interactive session.

## Examples

### Basic Commands

```text
$ date
12.10.2026.

$ time
21:30:00

$ prompt "my-cli> "
my-cli>
```

The displayed date and time depend on the system clock.

### File Operations

```text
$ touch example.txt
$ truncate example.txt
$ rm example.txt
```

- `touch` creates a new file.
- `truncate` empties an existing file.
- `rm` removes a file.

### Text Processing

Count words in a file:

```text
$ wc -w example.txt
```

Count characters:

```text
$ wc -c example.txt
```

Display the first three lines of a file:

```text
$ head -n3 example.txt
```

Replace occurrences of a string:

```text
$ tr example.txt -"old" "new"
```

### Output Redirection

Write command output to a file:

```text
$ time > output.txt
```

Append output instead of overwriting the existing contents:

```text
$ time >> output.txt
```

### Pipelines

The `|` operator connects commands by passing the output of one command to the input of the next.

```text
$ echo "hello world" | wc -w
2
```

Pipelines can combine supported commands to process data through multiple execution stages.

### Batch Execution

Create a text file containing commands, with one command per line:

```text
date
time
echo "Batch execution"
```

Save it as `commands.txt` and execute it with:

```text
$ batch commands.txt
```

The interpreter processes the commands sequentially and reports errors encountered during execution.

## Project Structure

```text
CLI-Interpreter/
├── commands/
│   ├── Batch.cpp / Batch.h
│   ├── Command.cpp / Command.h
│   ├── Date.cpp / Date.h
│   ├── Echo.cpp / Echo.h
│   ├── Head.cpp / Head.h
│   ├── Prompt.cpp / Prompt.h
│   ├── Rm.cpp / Rm.h
│   ├── Time.cpp / Time.h
│   ├── Touch.cpp / Touch.h
│   ├── Tr.cpp / Tr.h
│   ├── Truncate.cpp / Truncate.h
│   └── Wc.cpp / Wc.h
├── core/
│   ├── File_Handler.cpp / File_Handler.h
│   ├── Lexer.cpp / Lexer.h
│   └── Machine.cpp / Machine.h
├── utils/
│   ├── Constants.h
│   └── Exceptions.h
├── main.cpp
├── CMakeLists.txt
└── README.md
```

### Architecture

- **Lexer** — tokenizes command strings and identifies the command to execute.
- **Command** — abstract base class defining the common interface for all commands.
- **Individual commands** — implement their own argument parsing and execution logic.
- **Machine** — manages the interactive command loop, prompt, and pipeline execution.
- **File_Handler** — centralizes file reading and writing operations.
- **Exceptions** — defines specialized exceptions for different error conditions.

The command registry allows new commands to be integrated without redesigning the interpreter's core execution model.

## Technologies

- **Language:** C++17
- **Build system:** CMake
- **IDE:** CLion
- **Standard library:** STL, streams, file I/O, containers, and exception handling

## Building and Running

### Prerequisites

- A C++ compiler with C++17 support.
- CMake 3.28 or newer.
- An IDE such as CLion, or a compatible terminal environment.

### Build with CLion

1. Open the project directory in CLion.
2. Allow CLion to configure the CMake project.
3. Select the generated run configuration.
4. Build and run the executable.

### Build from the Terminal

From the project root, run:

```bash
cmake -S . -B build
cmake --build build
```

Run the generated executable from the build directory. Its exact path depends on your operating system and build configuration.

## Learning Objectives

This project explores several core concepts of systems-oriented programming:

- Object-oriented design and polymorphism.
- Lexical analysis and command parsing.
- Command dispatching and modular architecture.
- Standard input/output stream manipulation.
- Pipeline execution and stream buffering.
- File system operations.
- Exception-based error handling.
- Build configuration with CMake.

## Possible Improvements

Potential future extensions include:

- More complete shell syntax and argument parsing.
- Additional commands and command-line options.
- Improved quoting and escaping rules.
- More comprehensive automated tests.
- Expanded documentation and usage examples.

---

*An educational project exploring the design and implementation of a modular command-line interpreter in C++.*
