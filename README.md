# Enhanced Linux Shell

**OS Project - 2nd Year Engineering**  
**Authors:**  
- **Rishi C** (1RV24IS100)  
- **Nikhil K** (1RV24IS080)  
**Date:** January 25, 2026

---

## 📋 Project Overview

The **Enhanced Linux Shell** is a sophisticated command-line interpreter developed in C that replicates the core functionality of standard Unix shells (like Bash) while introducing enhanced user interface elements. It serves as a practical exploration of kernel-level interactions, process management, and system resource handling, specifically focusing on robust error handling and efficient memory allocation in a POSIX-compliant environment.

Unlike basic shell implementations, this project features **40 native built-in commands**, reducing reliance on external binaries for common tasks. It includes advanced features like I/O redirection, piping, background job management, and a persistent command history.

---

## ✨ Key Features

### 1. 🚀 40 Built-in Commands
A comprehensive suite of tools implemented directly within the shell:
- **File Operations:** `cp`, `mv`, `rm`, `touch`, `mkdir`, `rmdir`, `ls`, `chmod`, `stat`...
- **Text Processing:** `grep`, `head`, `tail`, `wc`, `sort`, `diff`...
- **System Info:** `whoami`, `hostname`, `uname`, `date`, `uptime`, `free`, `df`...
- **Process Management:** `ps`, `top`, `kill`, `jobs`, `bg`, `fg`...

### 2. 🎨 Enhanced UI
- **Color-coded Prompt:** Display user, host, and current directory in vibrant colors.
- **Visual Banners:** Custom ASCII art startup banner.
- **Formatted Output:** Colorized output for `ls`, `grep`, and error messages.

### 3. 🔄 I/O Redirection & Piping
- **Output Redirection (`>`):** Save command output to files (e.g., `ls > list.txt`).
- **Append Mode (`>>`):** Append output to existing files.
- **Input Redirection (`<`):** Read input from files.
- **Piping (`|`):** Chain commands together (e.g., `ps aux | grep shell`).

### 4. ⚡ Process Management
- **Background Jobs (`&`):** Run commands asynchronously.
- **Job Control:** List active jobs with `jobs` and monitor their status.
- **Signal Handling:** Proper cleanup of zombie processes using `SIGCHLD`.

### 5. 📜 History & Navigation
- **Command History:** Use `history` to view past commands.
- **Navigation:** `cd` with support for absolute/relative paths and home directory (`~`).

---

## 🔧 System Calls Used (20+)

The shell interacts directly with the Linux kernel using the following system calls:

| Category | System Calls |
|----------|--------------|
| **Process Control** | `fork()`, `execvp()`, `waitpid()`, `exit()`, `kill()`, `getpid()` |
| **File I/O** | `open()`, `close()`, `read()`, `write()`, `dup()`, `dup2()` |
| **File Management** | `stat()`, `lstat()`, `unlink()`, `rename()`, `chmod()`, `utime()` |
| **Directory Ops** | `opendir()`, `readdir()`, `closedir()`, `mkdir()`, `rmdir()`, `getcwd()`, `chdir()` |
| **Information** | `getuid()`, `gethostname()`, `uname()`, `time()` |

---

## 📥 Compilation & Usage

### 1. Compile
```bash
make
# OR
gcc -Wall -o shell shell.c
```

### 2. Run
```bash
./shell
```

### 3. Clean
```bash
make clean
```

---

## 💡 Usage Examples

### File Operations
```bash
# Copy a file
myshell> cp source.txt backup.txt

# Move/Rename a file
myshell> mv oldname.txt newname.txt

# Create a directory
myshell> mkdir project_files
```

### Text Processing & Piping
```bash
# Count lines in a file
myshell> wc -l data.txt

# Search for "error" in logs using pipe
myshell> cat app.log | grep error
```

### Background Jobs
```bash
# Run sleep in background
myshell> sleep 10 &
[1] 12345

# Check status
myshell> jobs
```

---

## 📁 Project Structure

```
simple-shell/
├── shell.c              # Main implementation (~1100 lines)
├── Makefile             # Build automation
├── README.md            # Project documentation
├── REPORT.md            # Detailed project report
└── docs/                # Additional documentation
```

---

## 📝 License

This project is created for educational purposes as part of the Operating Systems course at R.V. College of Engineering.
