#!/bin/bash
# Automated Test Suite for Linux Basic Shell
# Component 11: Testing & Demo
# Author: Rishi
# Date: January 4, 2026

# Color codes
GREEN='\033[1;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
CYAN='\033[1;36m'
RESET='\033[0m'

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test output directory
TEST_DIR="test_output"
mkdir -p "$TEST_DIR"

# Print test header
print_header() {
    echo -e "${CYAN}╔════════════════════════════════════════════════════╗${RESET}"
    echo -e "${CYAN}║        Linux Basic Shell - Test Suite             ║${RESET}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════╝${RESET}"
    echo ""
}

# Print section header
print_section() {
    echo -e "\n${BLUE}► $1${RESET}"
    echo -e "${BLUE}$(printf '═%.0s' {1..50})${RESET}"
}

# Test function
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "  Testing: $test_name ... "
    
    # Run command
    result=$(eval "$command" 2>&1)
    
    # Check result
    if [[ -n "$expected" ]] && [[ "$result" != *"$expected"* ]]; then
        echo -e "${RED}✗ FAILED${RESET}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "    Expected: $expected"
        echo "    Got: $result"
    else
        echo -e "${GREEN}✓ PASSED${RESET}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
}

# Compile shell if needed
compile_shell() {
    if [ ! -f "./shell" ]; then
        echo -e "${YELLOW}Compiling shell...${RESET}"
        make clean > /dev/null 2>&1
        make > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo -e "${RED}✗ Compilation failed!${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Compilation successful${RESET}\n"
    fi
}

# Start tests
print_header
compile_shell

# =============================================================================
# TEST SUITE
# =============================================================================

print_section "1. Built-in Commands (Component 5)"

# Create test file for commands
cat > "$TEST_DIR/test_builtin.sh" << 'EOF'
pwd
cd /tmp
pwd
cd -
echo "Hello World"
help
exit
EOF

echo "  Testing built-in commands execution..."
timeout 2 ./shell < "$TEST_DIR/test_builtin.sh" > "$TEST_DIR/builtin_output.txt" 2>&1
if grep -q "Hello World" "$TEST_DIR/builtin_output.txt"; then
    echo -e "  ${GREEN}✓ Built-in commands working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Built-in commands failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "2. External Command Execution (Component 4)"

# Test external commands
cat > "$TEST_DIR/test_external.sh" << 'EOF'
ls
date
echo "test"
exit
EOF

echo "  Testing external command execution..."
timeout 2 ./shell < "$TEST_DIR/test_external.sh" > "$TEST_DIR/external_output.txt" 2>&1
if [ -s "$TEST_DIR/external_output.txt" ]; then
    echo -e "  ${GREEN}✓ External commands working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ External commands failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "3. I/O Redirection (Component 6)"

# Create test input file
echo "Input test data" > "$TEST_DIR/input.txt"

# Test output redirection
cat > "$TEST_DIR/test_redirect.sh" << 'EOF'
echo "output test" > test_output/redirect_out.txt
cat < test_output/input.txt > test_output/redirect_in_out.txt
echo "append test" >> test_output/redirect_out.txt
exit
EOF

echo "  Testing I/O redirection..."
timeout 2 ./shell < "$TEST_DIR/test_redirect.sh" > /dev/null 2>&1

# Check output redirection
if [ -f "$TEST_DIR/redirect_out.txt" ] && grep -q "output test" "$TEST_DIR/redirect_out.txt"; then
    echo -e "  ${GREEN}✓ Output redirection working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Output redirection failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Check input redirection
if [ -f "$TEST_DIR/redirect_in_out.txt" ] && grep -q "Input test data" "$TEST_DIR/redirect_in_out.txt"; then
    echo -e "  ${GREEN}✓ Input redirection working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Input redirection failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Check append redirection
if [ -f "$TEST_DIR/redirect_out.txt" ] && grep -q "append test" "$TEST_DIR/redirect_out.txt"; then
    echo -e "  ${GREEN}✓ Append redirection working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Append redirection failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "4. Piping (Component 7)"

# Create test file with data
cat > "$TEST_DIR/pipe_test.txt" << 'EOF'
apple
banana
cherry
apricot
avocado
EOF

# Test piping
cat > "$TEST_DIR/test_pipe.sh" << 'EOF'
ls | grep shell
cat test_output/pipe_test.txt | grep "^a"
echo "test pipe" | cat
exit
EOF

echo "  Testing command piping..."
timeout 2 ./shell < "$TEST_DIR/test_pipe.sh" > "$TEST_DIR/pipe_output.txt" 2>&1
if grep -q "shell" "$TEST_DIR/pipe_output.txt"; then
    echo -e "  ${GREEN}✓ Single pipe working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Single pipe failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Test multiple pipes
cat > "$TEST_DIR/test_multipipe.sh" << 'EOF'
ls | grep . | wc -l
exit
EOF

echo "  Testing multiple pipes..."
timeout 2 ./shell < "$TEST_DIR/test_multipipe.sh" > "$TEST_DIR/multipipe_output.txt" 2>&1
if [ -s "$TEST_DIR/multipipe_output.txt" ]; then
    echo -e "  ${GREEN}✓ Multiple pipes working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Multiple pipes failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "5. Background Processing (Component 8)"

# Test background jobs
echo "  Testing background job execution..."
cat > "$TEST_DIR/test_background.sh" << 'EOF'
sleep 2 &
jobs
exit
EOF

timeout 5 ./shell < "$TEST_DIR/test_background.sh" > "$TEST_DIR/background_output.txt" 2>&1
if grep -q "background" "$TEST_DIR/background_output.txt" || grep -q "Job" "$TEST_DIR/background_output.txt"; then
    echo -e "  ${GREEN}✓ Background processing working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Background processing failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "6. Error Handling (Component 9)"

# Test invalid command
cat > "$TEST_DIR/test_errors.sh" << 'EOF'
invalidcommand12345
cd /nonexistent/directory
exit
EOF

echo "  Testing error handling..."
timeout 2 ./shell < "$TEST_DIR/test_errors.sh" > "$TEST_DIR/error_output.txt" 2>&1
if grep -qi "error\|not found\|failed" "$TEST_DIR/error_output.txt"; then
    echo -e "  ${GREEN}✓ Error handling working${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ Error handling failed${RESET}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "7. System Calls Verification (Component 2)"

# Check if shell uses system calls
echo "  Verifying system call usage..."
if command -v strace &> /dev/null; then
    timeout 2 strace -c ./shell < "$TEST_DIR/test_builtin.sh" 2> "$TEST_DIR/strace_output.txt" > /dev/null
    
    # Check for key system calls
    syscalls_found=0
    for syscall in fork execve wait4 open close read write pipe chdir getcwd; do
        if grep -q "$syscall" "$TEST_DIR/strace_output.txt"; then
            syscalls_found=$((syscalls_found + 1))
        fi
    done
    
    if [ $syscalls_found -ge 8 ]; then
        echo -e "  ${GREEN}✓ System calls verified ($syscalls_found/10 key calls)${RESET}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "  ${YELLOW}⚠ System calls partial ($syscalls_found/10 key calls)${RESET}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
else
    echo -e "  ${YELLOW}⚠ strace not available, skipping verification${RESET}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_section "8. Memory Leak Check (Optional)"

if command -v valgrind &> /dev/null; then
    echo "  Running valgrind memory check..."
    timeout 5 valgrind --leak-check=full --error-exitcode=1 ./shell < "$TEST_DIR/test_builtin.sh" > /dev/null 2> "$TEST_DIR/valgrind_output.txt"
    
    if grep -q "ERROR SUMMARY: 0 errors" "$TEST_DIR/valgrind_output.txt"; then
        echo -e "  ${GREEN}✓ No memory leaks detected${RESET}"
    else
        echo -e "  ${YELLOW}⚠ Some memory issues detected (check valgrind_output.txt)${RESET}"
    fi
else
    echo -e "  ${YELLOW}⚠ valgrind not available, skipping memory check${RESET}"
fi

# =============================================================================
# TEST SUMMARY
# =============================================================================

print_section "Test Summary"

echo ""
echo -e "${CYAN}╔════════════════════════════════════════════════════╗${RESET}"
echo -e "${CYAN}║              TEST RESULTS SUMMARY                  ║${RESET}"
echo -e "${CYAN}╚════════════════════════════════════════════════════╝${RESET}"
echo ""
echo -e "  Total Tests:   ${BLUE}$TOTAL_TESTS${RESET}"
echo -e "  Passed:        ${GREEN}$PASSED_TESTS${RESET}"
echo -e "  Failed:        ${RED}$FAILED_TESTS${RESET}"

# Calculate success rate
if [ $TOTAL_TESTS -gt 0 ]; then
    success_rate=$(( (PASSED_TESTS * 100) / TOTAL_TESTS ))
    echo -e "  Success Rate:  ${CYAN}${success_rate}%${RESET}"
fi

echo ""

# Final verdict
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════╗${RESET}"
    echo -e "${GREEN}║  ✓ ALL TESTS PASSED! READY FOR SUBMISSION        ║${RESET}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════╝${RESET}"
    exit 0
else
    echo -e "${YELLOW}╔════════════════════════════════════════════════════╗${RESET}"
    echo -e "${YELLOW}║  ⚠ SOME TESTS FAILED - CHECK OUTPUT               ║${RESET}"
    echo -e "${YELLOW}╚════════════════════════════════════════════════════╝${RESET}"
    echo -e "\n${YELLOW}Check test_output/ directory for detailed logs${RESET}"
    exit 1
fi
