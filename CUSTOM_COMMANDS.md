# Custom Commands Documentation

## 🎨 5 Unique Commands (Not in Standard Linux/Windows)

These are **custom-built commands** exclusive to our Enhanced Linux Shell, showcasing innovation and creativity in shell design.

---

### 1. **sysinfo** - Comprehensive System Information Dashboard

**Description:**  
A one-stop command that displays a beautifully formatted dashboard with complete system information, user details, and shell statistics.

**Usage:**
```bash
myshell> sysinfo
```

**What it shows:**
- 🖥️ **Operating System:** System name, release, version, and machine architecture
- 👤 **User Information:** Username, hostname, and user ID
- 📂 **Current Session:** Working directory, shell PID, and current time
- 📊 **Shell Statistics:** Number of commands in history and active background jobs

**Why it's unique:**  
Unlike standard Linux commands (`uname`, `whoami`, `hostname`) that show fragmented information, `sysinfo` aggregates everything into a single, color-coded, visually appealing display.

**Technical Implementation:**
- Uses `uname()` for OS info
- Uses `getpwuid()` and `getuid()` for user details
- Uses `gethostname()` for network name
- Uses `getcwd()` and `getpid()` for session info
- Accesses global shell variables for statistics

---

### 2. **tree** - Visual Directory Tree Structure

**Description:**  
Displays the contents of a directory in a tree-like structure with visual branches, making it easy to understand folder hierarchy at a glance.

**Usage:**
```bash
myshell> tree .
myshell> tree /home/user/projects
```

**Output Example:**
```
.
├── shell.c
├── Makefile
├── README.md
├── docs/
├── tests/

5 items
```

**Why it's unique:**  
While Linux has a `tree` command that requires installation, our built-in version is always available and uses Unicode box-drawing characters (├──) for a modern look. Directories are color-coded in blue.

**Technical Implementation:**
- Uses `opendir()` and `readdir()` to traverse directory
- Uses `stat()` to determine if entry is file or directory
- Uses `S_ISDIR()` macro for type checking
- Implements custom formatting with Unicode characters

---

### 3. **calc** - Built-in Calculator

**Description:**  
A command-line calculator that performs arithmetic operations without needing external tools like `bc` or `expr`.

**Usage:**
```bash
myshell> calc 10 + 5
myshell> calc 100 / 4
myshell> calc 7 * 8
myshell> calc 15 - 3
myshell> calc 17 % 5
```

**Output Example:**
```
╔═══ Calculator Result ═══╗
10.00 + 5.00 = 15.00
╚═════════════════════════╝
```

**Supported Operations:**
- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`*` or `x`)
- Division (`/`) with zero-division protection
- Modulo (`%`)

**Why it's unique:**  
Most shells require external programs for calculations. Our `calc` command is built-in, instant, and provides formatted output with error handling (e.g., division by zero).

**Technical Implementation:**
- Uses `atof()` to convert string arguments to floating-point numbers
- Implements operator parsing with `strcmp()`
- Provides formatted output with 2 decimal precision
- Includes comprehensive error checking

---

### 4. **reverse** - Reverse File Lines

**Description:**  
Reads a file and displays its contents in reverse order (last line first), useful for log analysis and data processing.

**Usage:**
```bash
myshell> reverse logfile.txt
myshell> reverse data.csv
```

**Input File (example.txt):**
```
Line 1
Line 2
Line 3
```

**Output:**
```
╔═══ Reversed File: example.txt ═══╗
Line 3
Line 2
Line 1
╚═══════════════════════════════════╝
```

**Why it's unique:**  
While Linux has `tac` (reverse of `cat`), it's not universally known or available on all systems. Our `reverse` command is intuitive, always available, and presents output in a formatted box.

**Technical Implementation:**
- Uses `fopen()` and `fgets()` to read file line by line
- Stores lines in a 2D array (supports up to 1000 lines)
- Iterates backward through the array for display
- Includes proper file error handling

---

### 5. **colortest** - Color Palette Display

**Description:**  
Tests and displays all available ANSI colors used in the shell, helping users understand the color scheme and verify terminal compatibility.

**Usage:**
```bash
myshell> colortest
```

**Output:**
```
╔═══════════════════════════════════════════════════╗
║           🎨 COLOR PALETTE TEST 🎨               ║
╚═══════════════════════════════════════════════════╝

■ RED     - Error messages and warnings
■ GREEN   - Success messages and confirmations
■ YELLOW  - Information and prompts
■ BLUE    - Directories and headers
■ MAGENTA - Special features and highlights
■ CYAN    - Borders and decorations
■ WHITE   - Standard text
■ BOLD    - Emphasis text

All colors working perfectly! ✓
```

**Why it's unique:**  
No standard shell has a built-in color testing utility. This command serves both as a diagnostic tool and a demonstration of the shell's enhanced UI capabilities.

**Technical Implementation:**
- Uses ANSI escape codes defined as macros (`COLOR_RED`, `COLOR_GREEN`, etc.)
- Displays each color with its purpose in the shell
- Provides visual confirmation of terminal color support

---

## 🎯 Why These Commands Matter for Your Presentation

1. **Innovation:** Shows you didn't just replicate existing commands but created new functionality
2. **Practical Use:** Each command solves a real problem or improves user experience
3. **Technical Depth:** Demonstrates mastery of:
   - File I/O operations
   - Directory traversal
   - String manipulation
   - Mathematical operations
   - ANSI terminal control
4. **User Experience:** All commands feature formatted, color-coded output
5. **Error Handling:** Each command includes proper validation and error messages

---

## 📝 How to Demo These in Your Presentation

### Demo Script:
```bash
# 1. Show comprehensive system info
myshell> sysinfo

# 2. Display directory structure
myshell> tree .

# 3. Perform calculations
myshell> calc 25 * 4
myshell> calc 100 / 3

# 4. Create a test file and reverse it
myshell> echo "First line" > test.txt
myshell> echo "Second line" >> test.txt
myshell> echo "Third line" >> test.txt
myshell> reverse test.txt

# 5. Test color support
myshell> colortest
```

---

## 🔑 Key Points to Mention

- **Total Commands:** 45 (40 standard + 5 custom unique)
- **Custom Commands:** Designed from scratch, not found in Bash/Zsh/Windows CMD
- **Purpose:** Enhance productivity and demonstrate advanced C programming
- **Integration:** Seamlessly integrated with existing shell architecture
- **Code Quality:** Each command includes error handling and user-friendly output

---

**These custom commands showcase creativity, technical skill, and user-centric design—perfect for impressing your evaluators! 🚀**
