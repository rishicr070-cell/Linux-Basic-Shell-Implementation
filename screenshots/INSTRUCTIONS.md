# Screenshot Instructions

**Component 11: Testing & Demo**  
Take these screenshots for complete documentation

---

## Required Screenshots

### 1. Compilation Process
**Filename:** `screenshots/01_compilation.png`  
**Command:** 
```bash
make clean
make
```
**What to capture:**
- Terminal showing compilation commands
- No errors
- "Build completed successfully" message
- Timestamp visible

---

### 2. Basic Commands
**Filename:** `screenshots/02_basic_commands.png`  
**Commands:**
```bash
./shell
pwd
ls
cd /tmp
pwd
cd -
echo "Hello World"
```
**What to capture:**
- Shell prompt with colors
- All command outputs
- Working directory changes

---

### 3. Built-in Commands & Help
**Filename:** `screenshots/03_builtin_help.png`  
**Commands:**
```bash
help
jobs
```
**What to capture:**
- Complete help menu display
- All built-in commands listed
- Advanced features section
- Jobs output (initially empty)

---

### 4. Output Redirection
**Filename:** `screenshots/04_output_redirect.png`  
**Commands:**
```bash
echo "test output" > test.txt
cat test.txt
ls -la > files.txt
cat files.txt
```
**What to capture:**
- Redirection working
- File creation
- Content verification

---

### 5. Append Redirection
**Filename:** `screenshots/05_append_redirect.png`  
**Commands:**
```bash
echo "line 1" > append.txt
echo "line 2" >> append.txt
echo "line 3" >> append.txt
cat append.txt
```
**What to capture:**
- Multiple appends
- All lines preserved
- Proper ordering

---

### 6. Input Redirection
**Filename:** `screenshots/06_input_redirect.png`  
**Commands:**
```bash
echo "input test data" > input.txt
cat < input.txt
wc -l < files.txt
```
**What to capture:**
- Input redirection working
- Data read from file
- Word count output

---

### 7. Single Pipe
**Filename:** `screenshots/07_single_pipe.png`  
**Commands:**
```bash
ls | grep .c
ls -l | grep shell
echo "test" | cat
ps aux | grep shell
```
**What to capture:**
- Various piping examples
- Filtered output
- Multiple test cases

---

### 8. Multiple Pipes
**Filename:** `screenshots/08_multiple_pipes.png`  
**Commands:**
```bash
ls | grep . | wc -l
cat test.txt | grep test | wc -l
ps aux | grep shell | wc -l
```
**What to capture:**
- Complex pipelines (3+ commands)
- Correct final output
- No errors

---

### 9. Background Jobs
**Filename:** `screenshots/09_background_jobs.png`  
**Commands:**
```bash
sleep 10 &
sleep 15 &
jobs
sleep 5 &
jobs
```
**What to capture:**
- Background job start messages
- Job IDs and PIDs
- Jobs list
- Job completion notifications

---

### 10. Error Handling
**Filename:** `screenshots/10_error_handling.png`  
**Commands:**
```bash
invalidcommand123
cd /nonexistent/directory
cat nonexistent_file.txt
```
**What to capture:**
- Error messages displayed
- Shell continues running
- Proper error formatting (red ERROR text)
- Shell doesn't crash

---

### 11. Combined Features
**Filename:** `screenshots/11_combined_features.png`  
**Commands:**
```bash
ls -la | grep .c > c_files.txt
cat c_files.txt
echo "background test" > bg_test.txt &
sleep 3 &
jobs
```
**What to capture:**
- Pipe + redirection together
- Background with redirection
- Multiple features working together

---

### 12. Testing Suite
**Filename:** `screenshots/12_automated_tests.png`  
**Commands:**
```bash
exit
make test
```
**What to capture:**
- Full test output
- All tests passing (green checkmarks)
- Test summary showing 100% pass rate
- "ALL TESTS PASSED" message

---

### 13. System Call Verification (Optional)
**Filename:** `screenshots/13_strace.png`  
**Commands:**
```bash
strace -c ./shell
pwd
ls
exit
```
**What to capture:**
- strace output showing system calls
- Key calls: fork, execve, wait4, open, close, pipe
- System call counts

---

### 14. Memory Check (Optional)
**Filename:** `screenshots/14_valgrind.png`  
**Commands:**
```bash
make valgrind
# Then in shell:
pwd
ls
exit
```
**What to capture:**
- valgrind output
- "All heap blocks were freed"
- "No leaks are possible"
- 0 errors

---

### 15. Project Structure
**Filename:** `screenshots/15_project_structure.png`  
**Commands:**
```bash
ls -la
tree
# or
find . -type f -name "*.c" -o -name "*.md" -o -name "Makefile"
cat README.md | head -20
```
**What to capture:**
- All project files
- Directory structure
- Documentation files present

---

## Tips for Good Screenshots

1. **Use Full Terminal Window**
   - Make terminal reasonably sized
   - Use clear, readable font (14pt+)
   - Dark theme with good contrast

2. **Show Colors**
   - Shell prompt colors visible
   - Error messages in red
   - Success messages in green
   - Help text colors

3. **Include Context**
   - Show full command
   - Show complete output
   - Don't crop important information
   - Include timestamp where relevant

4. **Clean Workspace**
   - Clear any personal information
   - Use test directory
   - Remove unrelated terminal output

5. **Organize Files**
   - Name files as specified above
   - Keep in screenshots/ directory
   - Compress if needed for submission

---

## Alternative: Screen Recording

Instead of screenshots, you can create a demo video:

**Filename:** `screenshots/demo_video.mp4`  
**Duration:** 3-5 minutes  
**Cover:**
- Compilation
- All basic commands
- Redirection (in, out, append)
- Piping (single and multiple)
- Background jobs
- Error handling
- Running tests

**Tools:**
- Linux: RecordMyDesktop, Kazam, OBS
- Windows: OBS, Xbox Game Bar
- Mac: QuickTime, ScreenFlow

---

## Quick Screenshot Script

Save this as `take_screenshots.sh`:

```bash
#!/bin/bash
# Automated screenshot helper
# Run in terminal: bash take_screenshots.sh

echo "Screenshot Guide for Linux Basic Shell"
echo "========================================"
echo ""
echo "1. Open this file alongside your terminal"
echo "2. Follow each section"
echo "3. Take screenshot after running commands"
echo "4. Save with suggested filename"
echo ""
echo "Press Enter after taking each screenshot..."

read -p "1. Compilation (make clean && make): " dummy
read -p "2. Basic commands (pwd, ls, cd): " dummy
read -p "3. Help & built-ins (help, jobs): " dummy
read -p "4. Output redirection (echo > file): " dummy
read -p "5. Append (echo >> file): " dummy
read -p "6. Input (cat < file): " dummy
read -p "7. Single pipe (ls | grep): " dummy
read -p "8. Multiple pipes (3+ commands): " dummy
read -p "9. Background jobs (sleep &, jobs): " dummy
read -p "10. Error handling (invalid commands): " dummy
read -p "11. Combined features: " dummy
read -p "12. Test suite (make test): " dummy

echo ""
echo "✓ All screenshots taken!"
echo "Check screenshots/ directory for completeness"
```

---

## Checklist

Before submission, verify:

- [ ] At least 10 screenshots taken
- [ ] All core features documented
- [ ] Error handling shown
- [ ] Tests passing screenshot included
- [ ] Files properly named
- [ ] Images are clear and readable
- [ ] No personal information visible
- [ ] All in screenshots/ directory

---

**Last Updated:** January 4, 2026  
**Status:** Ready for documentation
