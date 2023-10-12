# Shell Interpreter

## Overview
The Shell Interpreter is a custom Unix command-line shell designed to enhance command-line usability and functionality. It allows users to execute various commands, supports output redirection, piping, background processing, and error handling. This README provides an overview of the project and instructions on how to use it.

## Features

- Robust command parsing and execution capabilities
- Output redirection
- Piping of commands
- Background command processing
- Comprehensive error handling for invalid commands

## Getting Started

### Prerequisites
- Unix-like operating system (Linux, macOS, etc.)

### Installation
1. Clone this repository: `git clone ...`
2. Change into the project directory: `cd Shell-Terminal`
3. Build the shell interpreter: `make`

## Usage

### Running the Shell
To run the shell, simply execute the following commands:

1) ```gcc sshell```

2) ```./sshell```

### Basic Commands
You can use the following basic commands in the shell:
- `ls`: List files and directories in the current directory.
- `cd`: Change the current directory.
- `echo`: Display a message to the terminal.
- `pwd`: Print the current working directory.

### Advanced Features
The shell supports more advanced features like output redirection, piping, and background processing. Here are some examples:

- Redirect output to a file. For example: `ls > file.txt`
- Pipe two commands. For example: `ls | grep keyword`
- Run a command in the background by prepend `&`. For example: `&sleep(5)`

### Error Handling
If you enter an invalid command, the shell will provide a user-friendly error message.
