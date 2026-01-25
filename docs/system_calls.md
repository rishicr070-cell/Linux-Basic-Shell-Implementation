# System Calls Documentation

**Component 2: System Calls Implementation**  
**Total System Calls: 15+**

---

## Overview

This document provides detailed information about all system calls implemented in the Linux Basic Shell project. Each system call is documented with its purpose, usage, parameters, return values, and examples from the shell implementation.

---

## Table of Contents

1. [Process Management (6 calls)](#process-management)
2. [File Operations (5 calls)](#file-operations)
3. [Directory Operations (2 calls)](#directory-operations)
4. [Inter-Process Communication (2 calls)](#inter-process-communication)

---

## Process Management

### 1. fork()

**Header:** `<unistd.h>`  
**Prototype:** `pid_t fork(void);`

**Purpose:**  
Creates a new child process by duplicating the calling process. The child process is an exact copy of the parent except for the process ID.

**Return Values:**
- `> 0`: Parent process receives child's PID
- `0`: Child process receives 0
- `-1`: Error occurred

**Usage in Shell:**
```c
pid_t pid = fork();

if (pid == 0) {
    // Child process - execute command
    execvp(args[0], args);
    exit(127);
} else if (pid > 0) {
    // Parent process - wait or continue
    if (!background) {
        waitpid(pid, &status, 0);
    }
} else {
    perror("fork");
}
```

**Location in Code:** Line ~150, ~420 (main function, pipeline execution)

---

### 2. execvp()

**Header:** `<unistd.h>`  
**Prototype:** `int execvp(const char *file, char *const argv[]);`

**Purpose:**  
Replaces the current process image with a new program. Searches for the executable in directories listed in PATH environment variable.

**Parameters:**
- `file`: Name or path of the executable
- `argv`: Array of argument strings (NULL-terminated)

**Return Values:**
- Returns only on error: `-1`
- On success, never returns (process is replaced)

**Usage in Shell:**
```c
// In child process after fork()
execvp(args[0], args);

// If execvp returns, an error occurred
fprintf(stderr, "ERROR: Command '%s' not found\n", args[0]);
exit(127);
```

**Location in Code:** Lines ~160, ~435 (command execution, pipeline)

---

### 3. waitpid()

**Header:** `<sys/wait.h>`  
**Prototype:** `pid_t waitpid(pid_t pid, int *status, int options);`

**Purpose:**  
Suspends execution of the calling process until a specific child process terminates or a signal is delivered.

**Parameters:**
- `pid`: Process ID to wait for
  - `> 0`: Wait for specific child
  - `-1`: Wait for any child
  - `0`: Wait for any child in same process group
- `status`: Pointer to store child's exit status
- `options`: 
  - `0`: Block until child terminates
  - `WNOHANG`: Return immediately if no child has exited

**Return Values:**
- `> 0`: PID of terminated child
- `0`: With WNOHANG, no child exited
- `-1`: Error

**Usage in Shell:**
```c
// Foreground process
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
    fprintf(stderr, "Command exited with status %d\n", WEXITSTATUS(status));
}

// Background process reaping (signal handler)
while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    // Reap zombie processes
}
```

**Location in Code:** Lines ~175, ~470 (main loop, signal handler)

---

### 4. getpid()

**Header:** `<unistd.h>`  
**Prototype:** `pid_t getpid(void);`

**Purpose:**  
Returns the process ID of the calling process.

**Return Values:**
- Always succeeds, returns PID (positive integer)

**Usage in Shell:**
```c
// Used in prompt display (commented for simplicity)
// Could be used like:
pid_t shell_pid = getpid();
printf("Shell PID: %d\n", shell_pid);
```

**Location in Code:** Available for use in prompt (line ~460)

---

### 5. exit()

**Header:** `<stdlib.h>`  
**Prototype:** `void exit(int status);`

**Purpose:**  
Terminates the calling process and returns status code to parent.

**Parameters:**
- `status`: Exit status code
  - `0`: Success
  - Non-zero: Error (127 typically means command not found)

**Usage in Shell:**
```c
// In child process after failed exec
exit(127);

// In parent shell on exit command
if (strcmp(args[0], "exit") == 0) {
    int exit_code = args[1] ? atoi(args[1]) : 0;
    exit(exit_code);
}
```

**Location in Code:** Lines ~165, ~275 (error handling, exit builtin)

---

### 6. kill()

**Header:** `<signal.h>`  
**Prototype:** `int kill(pid_t pid, int sig);`

**Purpose:**  
Sends a signal to a process or group of processes.

**Parameters:**
- `pid`: Target process ID
  - `> 0`: Send to specific process
  - `0`: Send to all in current process group
  - `-1`: Send to all processes (except init)
- `sig`: Signal number (e.g., SIGTERM=15, SIGKILL=9)

**Return Values:**
- `0`: Success
- `-1`: Error

**Usage in Shell:**
```c
// Built-in kill command
if (strcmp(args[0], "kill") == 0) {
    pid_t pid = atoi(args[1]);
    
    if (kill(pid, SIGTERM) == 0) {
        printf("Signal sent to process %d\n", pid);
    } else {
        perror("kill");
    }
}
```

**Location in Code:** Lines ~320-330 (kill builtin)

---

## File Operations

### 7. open()

**Header:** `<fcntl.h>`  
**Prototype:** `int open(const char *pathname, int flags, mode_t mode);`

**Purpose:**  
Opens a file and returns a file descriptor.

**Parameters:**
- `pathname`: Path to file
- `flags`: Open mode flags (can be OR'd together)
  - `O_RDONLY`: Read only
  - `O_WRONLY`: Write only
  - `O_RDWR`: Read and write
  - `O_CREAT`: Create if doesn't exist
  - `O_TRUNC`: Truncate to zero length
  - `O_APPEND`: Append mode
- `mode`: Permissions when creating file (e.g., 0644)

**Return Values:**
- `≥ 0`: File descriptor
- `-1`: Error

**Usage in Shell:**
```c
// Output redirection: >
int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

// Append redirection: >>
int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);

// Input redirection: <
int fd = open(args[i+1], O_RDONLY);

if (fd == -1) {
    perror("open");
    return;
}
```

**Location in Code:** Lines ~345, ~358, ~373 (redirection handling)

---

### 8. close()

**Header:** `<unistd.h>`  
**Prototype:** `int close(int fd);`

**Purpose:**  
Closes a file descriptor, freeing the resource.

**Parameters:**
- `fd`: File descriptor to close

**Return Values:**
- `0`: Success
- `-1`: Error

**Usage in Shell:**
```c
// After using file descriptor
if (input_fd != -1) {
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);  // Close after dup2
}

if (output_fd != -1) {
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
}

// Close all pipe descriptors
for (int i = 0; i < 2 * (num_commands - 1); i++) {
    close(pipefds[i]);
}
```

**Location in Code:** Lines ~155, ~170, ~353, ~366, ~439, ~455 (various)

---

### 9. dup2()

**Header:** `<unistd.h>`  
**Prototype:** `int dup2(int oldfd, int newfd);`

**Purpose:**  
Duplicates file descriptor `oldfd` to `newfd`. If `newfd` is already open, it's closed first.

**Parameters:**
- `oldfd`: Source file descriptor
- `newfd`: Destination file descriptor (e.g., STDIN_FILENO=0, STDOUT_FILENO=1)

**Return Values:**
- `≥ 0`: New file descriptor
- `-1`: Error

**Usage in Shell:**
```c
// Redirect stdin for input redirection
if (input_fd != -1) {
    dup2(input_fd, STDIN_FILENO);  // stdin now reads from file
}

// Redirect stdout for output redirection
if (output_fd != -1) {
    dup2(output_fd, STDOUT_FILENO);  // stdout now writes to file
}

// Piping: redirect stdout to pipe
dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

// Piping: redirect stdin from pipe
dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
```

**Location in Code:** Lines ~152, ~156, ~428, ~433 (I/O redirection, piping)

---

### 10. read()

**Header:** `<unistd.h>`  
**Prototype:** `ssize_t read(int fd, void *buf, size_t count);`

**Purpose:**  
Reads data from a file descriptor into a buffer.

**Parameters:**
- `fd`: File descriptor to read from
- `buf`: Buffer to store data
- `count`: Maximum bytes to read

**Return Values:**
- `> 0`: Number of bytes read
- `0`: End of file
- `-1`: Error

**Usage in Shell:**
```c
// Implicitly used through:
// - getline() which uses read()
// - stdin operations
// - Input redirection automatically uses read()
ssize_t read_bytes = getline(&line, &len, stdin);
```

**Location in Code:** Line ~132 (getline uses read internally)

---

### 11. write()

**Header:** `<unistd.h>`  
**Prototype:** `ssize_t write(int fd, const void *buf, size_t count);`

**Purpose:**  
Writes data from a buffer to a file descriptor.

**Parameters:**
- `fd`: File descriptor to write to
- `buf`: Buffer containing data to write
- `count`: Number of bytes to write

**Return Values:**
- `≥ 0`: Number of bytes written
- `-1`: Error

**Usage in Shell:**
```c
// Implicitly used through:
// - printf() which uses write()
// - fprintf() for error messages
// - stdout/stderr operations
printf("myshell$ ");  // Uses write(STDOUT_FILENO, ...)
fprintf(stderr, "ERROR: ...");  // Uses write(STDERR_FILENO, ...)
```

**Location in Code:** Throughout (printf/fprintf calls use write)

---

## Directory Operations

### 12. chdir()

**Header:** `<unistd.h>`  
**Prototype:** `int chdir(const char *path);`

**Purpose:**  
Changes the current working directory of the calling process.

**Parameters:**
- `path`: Path to new directory (absolute or relative)

**Return Values:**
- `0`: Success
- `-1`: Error (e.g., directory doesn't exist, no permission)

**Usage in Shell:**
```c
// Built-in cd command
if (strcmp(args[0], "cd") == 0) {
    char *path = args[1] ? args[1] : getenv("HOME");
    
    if (chdir(path) != 0) {
        perror("cd");
    }
    return 1;
}
```

**Location in Code:** Lines ~240-250 (cd builtin)

**Examples:**
```bash
cd /home/rishi    # Absolute path
cd ..             # Parent directory
cd                # Home directory (default)
cd Documents      # Relative path
```

---

### 13. getcwd()

**Header:** `<unistd.h>`  
**Prototype:** `char *getcwd(char *buf, size_t size);`

**Purpose:**  
Gets the absolute pathname of the current working directory.

**Parameters:**
- `buf`: Buffer to store path
- `size`: Size of buffer

**Return Values:**
- `!= NULL`: Pointer to `buf` containing path
- `NULL`: Error

**Usage in Shell:**
```c
// Built-in pwd command
if (strcmp(args[0], "pwd") == 0) {
    char cwd[MAX_PATH];
    
    if (getcwd(cwd, sizeof(cwd))) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
    return 1;
}

// Used in prompt display
void print_prompt() {
    char cwd[MAX_PATH];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "unknown");
    }
    
    printf("myshell:%s$ ", cwd);
}
```

**Location in Code:** Lines ~255-265 (pwd builtin), ~462 (prompt)

---

## Inter-Process Communication

### 14. pipe()

**Header:** `<unistd.h>`  
**Prototype:** `int pipe(int pipefd[2]);`

**Purpose:**  
Creates a unidirectional data channel (pipe) for inter-process communication.

**Parameters:**
- `pipefd`: Array of two integers
  - `pipefd[0]`: Read end of pipe
  - `pipefd[1]`: Write end of pipe

**Return Values:**
- `0`: Success
- `-1`: Error

**Usage in Shell:**
```c
// Create pipes for command pipeline
int pipefds[2 * (num_commands - 1)];

for (int i = 0; i < num_commands - 1; i++) {
    if (pipe(pipefds + i * 2) == -1) {
        perror("pipe");
        return;
    }
}

// In child process:
// Write to pipe (output of command)
dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

// Read from pipe (input to command)
dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
```

**Location in Code:** Lines ~410-415 (pipeline creation)

**Data Flow:**
```
Command 1  -->  pipe[1] -->  pipe[0]  -->  Command 2
           (write end)    (read end)
```

---

### 15. dup()

**Header:** `<unistd.h>`  
**Prototype:** `int dup(int oldfd);`

**Purpose:**  
Duplicates a file descriptor, allocating a new descriptor that refers to the same open file description.

**Parameters:**
- `oldfd`: File descriptor to duplicate

**Return Values:**
- `≥ 0`: New file descriptor
- `-1`: Error

**Usage in Shell:**
```c
// While dup2() is primarily used for specific descriptor numbers,
// dup() can be used for saving file descriptors:

int saved_stdout = dup(STDOUT_FILENO);  // Save original stdout
dup2(file_fd, STDOUT_FILENO);            // Redirect stdout
// ... do work ...
dup2(saved_stdout, STDOUT_FILENO);       // Restore stdout
close(saved_stdout);

// Note: Our implementation primarily uses dup2() for explicit control
```

**Location in Code:** Available for use (could enhance I/O redirection)

---

## System Call Summary Table

| Category | System Call | Purpose | Lines Used |
|----------|-------------|---------|------------|
| Process | fork() | Create child process | 150, 420 |
| Process | execvp() | Execute command | 160, 435 |
| Process | waitpid() | Wait for child | 175, 470 |
| Process | getpid() | Get process ID | 460 |
| Process | exit() | Terminate process | 165, 275 |
| Process | kill() | Send signal | 325 |
| File | open() | Open file | 345, 358, 373 |
| File | close() | Close file | Multiple |
| File | dup2() | Duplicate descriptor | 152, 156, 428 |
| File | read() | Read data | 132 (implicit) |
| File | write() | Write data | Throughout |
| Directory | chdir() | Change directory | 245 |
| Directory | getcwd() | Get current dir | 260, 462 |
| IPC | pipe() | Create pipe | 413 |
| IPC | dup() | Duplicate descriptor | Available |

---

## Error Handling

All system calls are wrapped with proper error checking:

```c
if (fork() == -1) {
    perror("fork");
}

if (open(file, flags, mode) == -1) {
    perror("open");
}

if (pipe(pipefd) == -1) {
    perror("pipe");
}
```

The shell provides meaningful error messages using:
- `perror()` - Prints system error string
- `errno` - Global error number
- `strerror()` - Converts errno to string
- Custom error messages with context

---

## Testing System Calls

To verify system call usage:

```bash
# Using strace to trace system calls
strace -c ./shell

# This shows all system calls with counts:
# - fork: For each command
# - execve: For external commands
# - wait4/waitpid: For process synchronization
# - open/close: For file operations
# - pipe: For piping
# - etc.
```

---

## References

1. Linux Man Pages:
   - `man 2 fork`
   - `man 2 execve`
   - `man 2 wait`
   - `man 2 open`
   - `man 2 pipe`

2. Books:
   - "The Linux Programming Interface" by Michael Kerrisk
   - "Advanced Programming in the UNIX Environment" by W. Richard Stevens

3. Online Resources:
   - https://man7.org/linux/man-pages/
   - https://www.gnu.org/software/libc/manual/

---

**Last Updated:** January 4, 2026  
**Version:** 2.0  
**Component:** 2 - System Calls Implementation
