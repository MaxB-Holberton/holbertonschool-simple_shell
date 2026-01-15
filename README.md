C - Simple Shell Project
by Max Brook and Matthew Wirski C28

Our shell (H_Shell) is a simple UNIX command-line shell written in C. 
The goal of the project was to make a shell capable of  replicating basic shell behaviours like reading user input, parsing commands,
searching the PATH, forking new processes and executing commands using execve.

// As such, our programs key features are:

- Executing commands using fork() and execve()
- Searching executables in the PATH environment variable
- Interactive and Non-interactive modes
- Supporting absolute and relative paths
- Handling invalid commands
- Handling built-in commands (exit and env)

// Flow of how it works:

1. Display prompt ([H_Shell}$) when in interactive mode
2. Read user input using getline()
3. Parse input into arguments 
4. Handle built-in commands 
5. Search for executable in both direct path and each directory listed in PATH
6. Create child process using fork()
7. Execute command using execve()
8. Parent waits for child to finish

// Project structure:

env.c - Holds helper functions, parsing and env handling
shell.c - Contains the main shell code (main loop and entry)
shell.h - Header file containing all libraries required for the project 
argv_creation.c - Handles parsing input into arguments
AUTHORS - Authors page
README.md - Readme file containing what you are reading
man_1_simple_shell - Manual for our project

// Usage

Interactive Mode:

./hsh /* Activates the shell */
[H_Shell]$ ls
[H_Shell]$ pwd
[H_Shell]$ exit

Non-Interactive Mode:

echo "ls -l" | ./hsh

// Built-in Commands

exit - exits the shell
env - Prints environment variables

// Error Handling

If a command cannot be found, the shell prints:
"./hsh: 1: command: not found"
Then it returns exit status 127 by using _exit(127) when execve() fails in the child process.

