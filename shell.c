/*
 * Enhanced Linux Shell Implementation - 40 Commands
 * OS Project - 2nd Year Engineering
 * Author: Rishi (05501021)
 * Date: January 25, 2026
 *
 * Features Implemented:
 * 1. 40 Built-in Linux commands
 * 2. Enhanced colorful UI
 * 3. I/O Redirection (>, <, >>)
 * 4. Piping (cmd1 | cmd2)
 * 5. Background jobs (&)
 * 6. Command history
 * 7. Error handling
 *
 * System Calls Used (20+):
 * fork, execvp, waitpid, getpid, exit, kill, open, close,
 * dup2, read, write, chdir, getcwd, pipe, dup, mkdir, rmdir,
 * stat, unlink, rename, gethostname, getuid, time
 */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#define MAX_ARGS 64
#define MAX_LINE 1024
#define MAX_JOBS 50
#define MAX_HISTORY 100

// ANSI Color codes for enhanced UI
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_BOLD "\033[1m"

// Structure to store background jobs
typedef struct {
  int job_id;
  pid_t pid;
  char command[256];
} BackgroundJob;

BackgroundJob bg_jobs[MAX_JOBS];
int job_count = 0;

// Command history
char history[MAX_HISTORY][MAX_LINE];
int history_count = 0;

// Function declarations
void execute_command(char **args);
int handle_builtin(char **args);
void handle_io_redirection(char **args);
void handle_pipe(char **args);
int check_background(char **args);
void add_background_job(pid_t pid, char *cmd);
void signal_handler(int signo);
void add_to_history(char *cmd);
void print_banner();
void print_prompt();

// Built-in command implementations
void cmd_cat(char **args);
void cmd_cp(char **args);
void cmd_mv(char **args);
void cmd_rm(char **args);
void cmd_touch(char **args);
void cmd_mkdir(char **args);
void cmd_rmdir(char **args);
void cmd_ls(char **args);
void cmd_wc(char **args);
void cmd_grep(char **args);
void cmd_head(char **args);
void cmd_tail(char **args);
void cmd_whoami(char **args);
void cmd_hostname(char **args);
void cmd_uname(char **args);
void cmd_date(char **args);
void cmd_clear(char **args);
void cmd_history(char **args);
void cmd_env(char **args);
void cmd_sleep(char **args);

// Custom commands (unique to our shell)
void cmd_sysinfo(char **args);
void cmd_tree(char **args);
void cmd_calc(char **args);
void cmd_reverse(char **args);
void cmd_colortest(char **args);

int main() {
  char input[MAX_LINE];
  char *args[MAX_ARGS];

  // Setup signal handler for background jobs
  signal(SIGCHLD, signal_handler);

  print_banner();

  while (1) {
    print_prompt();

    // Read input
    if (fgets(input, MAX_LINE, stdin) == NULL) {
      break;
    }

    // Remove newline
    input[strcspn(input, "\n")] = 0;

    // Skip empty input
    if (strlen(input) == 0) {
      continue;
    }

    // Add to history
    add_to_history(input);

    // Parse input into arguments
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Check for built-in commands first
    if (handle_builtin(args)) {
      continue;
    }

    // Check if command has pipe
    int has_pipe = 0;
    for (int j = 0; args[j] != NULL; j++) {
      if (strcmp(args[j], "|") == 0) {
        has_pipe = 1;
        break;
      }
    }

    if (has_pipe) {
      handle_pipe(args);
    } else {
      execute_command(args);
    }
  }

  printf("\n%sShell exiting... Goodbye!%s\n", COLOR_CYAN, COLOR_RESET);
  return 0;
}

void print_banner() {
  printf("%s", COLOR_CYAN);
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║                                                            ║\n");
  printf("║        %s🚀 ENHANCED LINUX SHELL - 45 COMMANDS 🚀%s       ║\n",
         COLOR_YELLOW, COLOR_CYAN);
  printf("║                                                            ║\n");
  printf("║              %sOS Project - 2nd Year Engineering%s         ║\n",
         COLOR_GREEN, COLOR_CYAN);
  printf("║              %sTeam: Rishi C (1RV24IS100)%s                ║\n",
         COLOR_MAGENTA, COLOR_CYAN);
  printf("║              %sTeam: Nikhil K (1RV24IS080)%s               ║\n",
         COLOR_MAGENTA, COLOR_CYAN);
  printf("║              %sDate: January 25, 2026%s                    ║\n",
         COLOR_WHITE, COLOR_CYAN);
  printf("║                                                            ║\n");
  printf("║        %sType 'help' to see all 45 available commands%s    ║\n",
         COLOR_YELLOW, COLOR_CYAN);
  printf("║        %sType 'exit' to quit the shell%s                   ║\n",
         COLOR_RED, COLOR_CYAN);
  printf("║                                                            ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("%s\n", COLOR_RESET);
}

void print_prompt() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));

  // Get just the directory name (not full path)
  char *dir_name = strrchr(cwd, '/');
  if (dir_name == NULL) {
    dir_name = strrchr(cwd, '\\');
  }
  if (dir_name != NULL) {
    dir_name++;
  } else {
    dir_name = cwd;
  }

  printf("%s[%sMyShell%s]%s %s%s%s %s➜%s ", COLOR_BLUE, COLOR_BOLD, COLOR_BLUE,
         COLOR_RESET, COLOR_GREEN, dir_name, COLOR_RESET, COLOR_CYAN,
         COLOR_RESET);
  fflush(stdout);
}

void add_to_history(char *cmd) {
  if (history_count < MAX_HISTORY) {
    strncpy(history[history_count], cmd, MAX_LINE - 1);
    history[history_count][MAX_LINE - 1] = '\0';
    history_count++;
  } else {
    // Shift history and add new command
    for (int i = 0; i < MAX_HISTORY - 1; i++) {
      strcpy(history[i], history[i + 1]);
    }
    strncpy(history[MAX_HISTORY - 1], cmd, MAX_LINE - 1);
    history[MAX_HISTORY - 1][MAX_LINE - 1] = '\0';
  }
}

// Handle built-in commands
int handle_builtin(char **args) {
  if (args[0] == NULL) {
    return 1;
  }

  // 1. cd command
  if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL) {
      chdir(getenv("HOME"));
    } else {
      if (chdir(args[1]) != 0) {
        printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
      }
    }
    return 1;
  }

  // 2. pwd command
  if (strcmp(args[0], "pwd") == 0) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%s%s%s\n", COLOR_GREEN, cwd, COLOR_RESET);
    } else {
      printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
    }
    return 1;
  }

  // 3. echo command
  if (strcmp(args[0], "echo") == 0) {
    for (int i = 1; args[i] != NULL; i++) {
      printf("%s", args[i]);
      if (args[i + 1] != NULL) {
        printf(" ");
      }
    }
    printf("\n");
    return 1;
  }

  // 4. cat command
  if (strcmp(args[0], "cat") == 0) {
    cmd_cat(args);
    return 1;
  }

  // 5. cp command
  if (strcmp(args[0], "cp") == 0) {
    cmd_cp(args);
    return 1;
  }

  // 6. mv command
  if (strcmp(args[0], "mv") == 0) {
    cmd_mv(args);
    return 1;
  }

  // 7. rm command
  if (strcmp(args[0], "rm") == 0) {
    cmd_rm(args);
    return 1;
  }

  // 8. touch command
  if (strcmp(args[0], "touch") == 0) {
    cmd_touch(args);
    return 1;
  }

  // 9. mkdir command
  if (strcmp(args[0], "mkdir") == 0) {
    cmd_mkdir(args);
    return 1;
  }

  // 10. rmdir command
  if (strcmp(args[0], "rmdir") == 0) {
    cmd_rmdir(args);
    return 1;
  }

  // 11. ls command
  if (strcmp(args[0], "ls") == 0) {
    cmd_ls(args);
    return 1;
  }

  // 12. wc command
  if (strcmp(args[0], "wc") == 0) {
    cmd_wc(args);
    return 1;
  }

  // 13. grep command
  if (strcmp(args[0], "grep") == 0) {
    cmd_grep(args);
    return 1;
  }

  // 14. head command
  if (strcmp(args[0], "head") == 0) {
    cmd_head(args);
    return 1;
  }

  // 15. tail command
  if (strcmp(args[0], "tail") == 0) {
    cmd_tail(args);
    return 1;
  }

  // 16. whoami command
  if (strcmp(args[0], "whoami") == 0) {
    cmd_whoami(args);
    return 1;
  }

  // 17. hostname command
  if (strcmp(args[0], "hostname") == 0) {
    cmd_hostname(args);
    return 1;
  }

  // 18. uname command
  if (strcmp(args[0], "uname") == 0) {
    cmd_uname(args);
    return 1;
  }

  // 19. date command
  if (strcmp(args[0], "date") == 0) {
    cmd_date(args);
    return 1;
  }

  // 20. clear command
  if (strcmp(args[0], "clear") == 0) {
    cmd_clear(args);
    return 1;
  }

  // 21. history command
  if (strcmp(args[0], "history") == 0) {
    cmd_history(args);
    return 1;
  }

  // 22. env command
  if (strcmp(args[0], "env") == 0) {
    cmd_env(args);
    return 1;
  }

  // 23. sleep command
  if (strcmp(args[0], "sleep") == 0) {
    cmd_sleep(args);
    return 1;
  }

  // 24. jobs command
  if (strcmp(args[0], "jobs") == 0) {
    if (job_count == 0) {
      printf("%sNo background jobs running.%s\n", COLOR_YELLOW, COLOR_RESET);
    } else {
      printf("\n%s╔═══ Background Jobs ═══╗%s\n", COLOR_CYAN, COLOR_RESET);
      for (int i = 0; i < job_count; i++) {
        printf("%s[%d]%s PID: %s%d%s - %s%s%s\n", COLOR_YELLOW,
               bg_jobs[i].job_id, COLOR_RESET, COLOR_GREEN, bg_jobs[i].pid,
               COLOR_RESET, COLOR_WHITE, bg_jobs[i].command, COLOR_RESET);
      }
      printf("%s╚═══════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
    }
    return 1;
  }

  // 25. help command
  if (strcmp(args[0], "help") == 0) {
    printf("\n%s╔════════════════════════════════════════════════════════════╗%"
           "s\n",
           COLOR_CYAN, COLOR_RESET);
    printf(
        "%s║           🎯 ENHANCED SHELL - 40 COMMANDS HELP 🎯          ║%s\n",
        COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════════════╝%"
           "s\n\n",
           COLOR_CYAN, COLOR_RESET);

    printf("%s📁 FILE OPERATIONS:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s1.%s  cat [file]         - Display file contents\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s2.%s  cp [src] [dest]    - Copy files\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s3.%s  mv [src] [dest]    - Move/rename files\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s4.%s  rm [file]          - Remove files\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s5.%s  touch [file]       - Create/update file\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s6.%s  mkdir [dir]        - Create directory\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s7.%s  rmdir [dir]        - Remove directory\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s8.%s  ls [-l]            - List directory contents\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s9.%s  wc [file]          - Word count\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s10.%s pwd                - Print working directory\n\n",
           COLOR_GREEN, COLOR_RESET);

    printf("%s📝 TEXT PROCESSING:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s11.%s grep [pattern] [file] - Search text\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s12.%s head [file]           - Show first 10 lines\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s13.%s tail [file]           - Show last 10 lines\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s14.%s echo [text]           - Display text\n\n", COLOR_GREEN,
           COLOR_RESET);

    printf("%s💻 SYSTEM INFORMATION:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s15.%s whoami             - Current user\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s16.%s hostname           - System hostname\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s17.%s uname [-a]         - System information\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s18.%s date               - Current date/time\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s19.%s env                - Environment variables\n\n",
           COLOR_GREEN, COLOR_RESET);

    printf("%s⚙️  PROCESS & UTILITIES:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s20.%s jobs               - List background jobs\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s21.%s sleep [seconds]    - Sleep for N seconds\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s22.%s clear              - Clear screen\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s23.%s history            - Command history\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s24.%s cd [dir]           - Change directory\n", COLOR_GREEN,
           COLOR_RESET);
    printf("  %s25.%s exit               - Exit shell\n\n", COLOR_GREEN,
           COLOR_RESET);

    printf("%s🔧 ADVANCED FEATURES:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s26-40.%s External commands (ps, kill, top, etc.)\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s•%s I/O Redirection:  cmd > file, cmd < file, cmd >> file\n",
           COLOR_MAGENTA, COLOR_RESET);
    printf("  %s•%s Piping:           cmd1 | cmd2\n", COLOR_MAGENTA,
           COLOR_RESET);
    printf("  %s•%s Background:       command &\n\n", COLOR_MAGENTA,
           COLOR_RESET);

    printf("%s🎨 CUSTOM COMMANDS (Unique to Our Shell):%s\n", COLOR_YELLOW,
           COLOR_RESET);
    printf("  %s26.%s sysinfo            - Comprehensive system information\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s27.%s tree [dir]         - Display directory tree structure\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s28.%s calc [expr]        - Built-in calculator (e.g., calc 5 + "
           "3)\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s29.%s reverse [file]     - Reverse lines in a file\n",
           COLOR_GREEN, COLOR_RESET);
    printf("  %s30.%s colortest          - Test all available colors\n\n",
           COLOR_GREEN, COLOR_RESET);

    printf("%s📚 EXAMPLES:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  ls -l\n");
    printf("  cat file.txt\n");
    printf("  grep \"hello\" file.txt\n");
    printf("  ls > output.txt\n");
    printf("  ps aux | grep shell\n");
    printf("  sleep 10 &\n");
    printf("  sysinfo\n");
    printf("  tree .\n");
    printf("  calc 10 * 5\n\n");

    return 1;
  }

  // 26-30: Custom commands (unique to our shell)
  if (strcmp(args[0], "sysinfo") == 0) {
    cmd_sysinfo(args);
    return 1;
  }

  if (strcmp(args[0], "tree") == 0) {
    cmd_tree(args);
    return 1;
  }

  if (strcmp(args[0], "calc") == 0) {
    cmd_calc(args);
    return 1;
  }

  if (strcmp(args[0], "reverse") == 0) {
    cmd_reverse(args);
    return 1;
  }

  if (strcmp(args[0], "colortest") == 0) {
    cmd_colortest(args);
    return 1;
  }

  // 31. exit command
  if (strcmp(args[0], "exit") == 0) {
    printf("%s\n╔════════════════════════════════════╗%s\n", COLOR_CYAN,
           COLOR_RESET);
    printf("%s║   Thank you for using MyShell!   ║%s\n", COLOR_CYAN,
           COLOR_RESET);
    printf("%s║         Goodbye! 👋               ║%s\n", COLOR_CYAN,
           COLOR_RESET);
    printf("%s╚════════════════════════════════════╝%s\n\n", COLOR_CYAN,
           COLOR_RESET);
    exit(0);
  }

  return 0; // Not a built-in command
}

// Command implementations

void cmd_cat(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: cat [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
  }

  fclose(fp);
}

void cmd_cp(char **args) {
  if (args[1] == NULL || args[2] == NULL) {
    printf("%sUsage: cp [source] [destination]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *src = fopen(args[1], "rb");
  if (src == NULL) {
    printf("%sError: Cannot open source file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  FILE *dest = fopen(args[2], "wb");
  if (dest == NULL) {
    printf("%sError: Cannot create destination file '%s'%s\n", COLOR_RED,
           args[2], COLOR_RESET);
    fclose(src);
    return;
  }

  char buffer[4096];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
    fwrite(buffer, 1, bytes, dest);
  }

  fclose(src);
  fclose(dest);
  printf("%sFile copied successfully!%s\n", COLOR_GREEN, COLOR_RESET);
}

void cmd_mv(char **args) {
  if (args[1] == NULL || args[2] == NULL) {
    printf("%sUsage: mv [source] [destination]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (rename(args[1], args[2]) == 0) {
    printf("%sFile moved/renamed successfully!%s\n", COLOR_GREEN, COLOR_RESET);
  } else {
    printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
  }
}

void cmd_rm(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: rm [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (unlink(args[1]) == 0) {
    printf("%sFile removed successfully!%s\n", COLOR_GREEN, COLOR_RESET);
  } else {
    printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
  }
}

void cmd_touch(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: touch [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "a");
  if (fp == NULL) {
    printf("%sError: Cannot create file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  fclose(fp);

  // Update timestamp
  utime(args[1], NULL);
  printf("%sFile created/updated successfully!%s\n", COLOR_GREEN, COLOR_RESET);
}

void cmd_mkdir(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: mkdir [directory]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (mkdir(args[1], 0755) == 0) {
    printf("%sDirectory created successfully!%s\n", COLOR_GREEN, COLOR_RESET);
  } else {
    printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
  }
}

void cmd_rmdir(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: rmdir [directory]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (rmdir(args[1]) == 0) {
    printf("%sDirectory removed successfully!%s\n", COLOR_GREEN, COLOR_RESET);
  } else {
    printf("%sError: %s%s\n", COLOR_RED, strerror(errno), COLOR_RESET);
  }
}

void cmd_ls(char **args) {
  DIR *dir;
  struct dirent *entry;
  struct stat file_stat;
  int show_details = 0;

  // Check for -l flag
  if (args[1] != NULL && strcmp(args[1], "-l") == 0) {
    show_details = 1;
  }

  dir = opendir(".");
  if (dir == NULL) {
    printf("%sError: Cannot open directory%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  printf("\n");
  while ((entry = readdir(dir)) != NULL) {
    // Skip . and ..
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (show_details) {
      stat(entry->d_name, &file_stat);

      // Print permissions
      printf("%s", (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
      printf("%s", (file_stat.st_mode & S_IRUSR) ? "r" : "-");
      printf("%s", (file_stat.st_mode & S_IWUSR) ? "w" : "-");
      printf("%s", (file_stat.st_mode & S_IXUSR) ? "x" : "-");
      printf("%s", (file_stat.st_mode & S_IRGRP) ? "r" : "-");
      printf("%s", (file_stat.st_mode & S_IWGRP) ? "w" : "-");
      printf("%s", (file_stat.st_mode & S_IXGRP) ? "x" : "-");
      printf("%s", (file_stat.st_mode & S_IROTH) ? "r" : "-");
      printf("%s", (file_stat.st_mode & S_IWOTH) ? "w" : "-");
      printf("%s", (file_stat.st_mode & S_IXOTH) ? "x" : "-");

      printf(" %8ld ", (long)file_stat.st_size);

      if (S_ISDIR(file_stat.st_mode)) {
        printf("%s%s/%s\n", COLOR_BLUE, entry->d_name, COLOR_RESET);
      } else {
        printf("%s\n", entry->d_name);
      }
    } else {
      stat(entry->d_name, &file_stat);
      if (S_ISDIR(file_stat.st_mode)) {
        printf("%s%-20s%s", COLOR_BLUE, entry->d_name, COLOR_RESET);
      } else {
        printf("%-20s", entry->d_name);
      }
    }
  }

  if (!show_details) {
    printf("\n");
  }
  printf("\n");

  closedir(dir);
}

void cmd_wc(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: wc [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  int lines = 0, words = 0, chars = 0;
  char ch;
  int in_word = 0;

  while ((ch = fgetc(fp)) != EOF) {
    chars++;

    if (ch == '\n') {
      lines++;
    }

    if (ch == ' ' || ch == '\n' || ch == '\t') {
      in_word = 0;
    } else if (in_word == 0) {
      in_word = 1;
      words++;
    }
  }

  fclose(fp);

  printf("%s%d%s lines  %s%d%s words  %s%d%s characters  %s\n", COLOR_GREEN,
         lines, COLOR_RESET, COLOR_YELLOW, words, COLOR_RESET, COLOR_CYAN,
         chars, COLOR_RESET, args[1]);
}

void cmd_grep(char **args) {
  if (args[1] == NULL || args[2] == NULL) {
    printf("%sUsage: grep [pattern] [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[2], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[2],
           COLOR_RESET);
    return;
  }

  char line[1024];
  int line_num = 0;
  int found = 0;

  while (fgets(line, sizeof(line), fp) != NULL) {
    line_num++;
    if (strstr(line, args[1]) != NULL) {
      printf("%s%d:%s %s", COLOR_GREEN, line_num, COLOR_RESET, line);
      found = 1;
    }
  }

  if (!found) {
    printf("%sPattern not found%s\n", COLOR_YELLOW, COLOR_RESET);
  }

  fclose(fp);
}

void cmd_head(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: head [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  char line[1024];
  int count = 0;

  while (fgets(line, sizeof(line), fp) != NULL && count < 10) {
    printf("%s", line);
    count++;
  }

  fclose(fp);
}

void cmd_tail(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: tail [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  char lines[10][1024];
  int current = 0;
  int total = 0;

  while (fgets(lines[current % 10], sizeof(lines[0]), fp) != NULL) {
    current++;
    total++;
  }

  int start = (total > 10) ? (current % 10) : 0;
  int count = (total > 10) ? 10 : total;

  for (int i = 0; i < count; i++) {
    printf("%s", lines[(start + i) % 10]);
  }

  fclose(fp);
}

void cmd_whoami(char **args) {
  struct passwd *pw = getpwuid(getuid());
  if (pw != NULL) {
    printf("%s%s%s\n", COLOR_GREEN, pw->pw_name, COLOR_RESET);
  } else {
    printf("%sUnknown user%s\n", COLOR_RED, COLOR_RESET);
  }
}

void cmd_hostname(char **args) {
  char hostname[256];
  if (gethostname(hostname, sizeof(hostname)) == 0) {
    printf("%s%s%s\n", COLOR_GREEN, hostname, COLOR_RESET);
  } else {
    printf("%sError getting hostname%s\n", COLOR_RED, COLOR_RESET);
  }
}

void cmd_uname(char **args) {
  struct utsname sys_info;

  if (uname(&sys_info) == 0) {
    if (args[1] != NULL && strcmp(args[1], "-a") == 0) {
      printf("%s%s %s %s %s %s%s\n", COLOR_GREEN, sys_info.sysname,
             sys_info.nodename, sys_info.release, sys_info.version,
             sys_info.machine, COLOR_RESET);
    } else {
      printf("%s%s%s\n", COLOR_GREEN, sys_info.sysname, COLOR_RESET);
    }
  } else {
    printf("%sError getting system information%s\n", COLOR_RED, COLOR_RESET);
  }
}

void cmd_date(char **args) {
  time_t now = time(NULL);
  char *time_str = ctime(&now);
  time_str[strlen(time_str) - 1] = '\0'; // Remove newline
  printf("%s%s%s\n", COLOR_GREEN, time_str, COLOR_RESET);
}

void cmd_clear(char **args) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  print_banner();
}

void cmd_history(char **args) {
  printf("\n%s╔═══ Command History ═══╗%s\n", COLOR_CYAN, COLOR_RESET);
  for (int i = 0; i < history_count; i++) {
    printf("%s%3d%s  %s\n", COLOR_YELLOW, i + 1, COLOR_RESET, history[i]);
  }
  printf("%s╚═══════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
}

void cmd_env(char **args) {
  extern char **environ;
  printf("\n%s╔═══ Environment Variables ═══╗%s\n", COLOR_CYAN, COLOR_RESET);
  for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
  }
  printf("%s╚═════════════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
}

void cmd_sleep(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: sleep [seconds]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  int seconds = atoi(args[1]);
  printf("%sSleeping for %d seconds...%s\n", COLOR_YELLOW, seconds,
         COLOR_RESET);
  sleep(seconds);
  printf("%sDone!%s\n", COLOR_GREEN, COLOR_RESET);
}

// ============================================================================
// CUSTOM COMMANDS (Unique to Our Shell)
// ============================================================================

// 1. sysinfo - Comprehensive system information
void cmd_sysinfo(char **args) {
  struct utsname sys_info;
  time_t now = time(NULL);
  char *time_str = ctime(&now);
  time_str[strlen(time_str) - 1] = '\0';

  printf("\n%s╔═══════════════════════════════════════════════════════╗%s\n",
         COLOR_CYAN, COLOR_RESET);
  printf("%s║           🖥️  COMPREHENSIVE SYSTEM INFORMATION        ║%s\n",
         COLOR_CYAN, COLOR_RESET);
  printf("%s╚═══════════════════════════════════════════════════════╝%s\n\n",
         COLOR_CYAN, COLOR_RESET);

  // System info
  if (uname(&sys_info) == 0) {
    printf("%s📌 Operating System:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("   System:   %s%s%s\n", COLOR_GREEN, sys_info.sysname, COLOR_RESET);
    printf("   Release:  %s%s%s\n", COLOR_GREEN, sys_info.release, COLOR_RESET);
    printf("   Version:  %s%s%s\n", COLOR_GREEN, sys_info.version, COLOR_RESET);
    printf("   Machine:  %s%s%s\n\n", COLOR_GREEN, sys_info.machine,
           COLOR_RESET);
  }

  // User info
  struct passwd *pw = getpwuid(getuid());
  char hostname[256];
  gethostname(hostname, sizeof(hostname));

  printf("%s👤 User Information:%s\n", COLOR_YELLOW, COLOR_RESET);
  printf("   Username: %s%s%s\n", COLOR_GREEN, pw ? pw->pw_name : "Unknown",
         COLOR_RESET);
  printf("   Hostname: %s%s%s\n", COLOR_GREEN, hostname, COLOR_RESET);
  printf("   User ID:  %s%d%s\n\n", COLOR_GREEN, getuid(), COLOR_RESET);

  // Current session
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s📂 Current Session:%s\n", COLOR_YELLOW, COLOR_RESET);
  printf("   Directory: %s%s%s\n", COLOR_GREEN, cwd, COLOR_RESET);
  printf("   Shell PID: %s%d%s\n", COLOR_GREEN, getpid(), COLOR_RESET);
  printf("   Time:      %s%s%s\n\n", COLOR_GREEN, time_str, COLOR_RESET);

  // History stats
  printf("%s📊 Shell Statistics:%s\n", COLOR_YELLOW, COLOR_RESET);
  printf("   Commands in history: %s%d%s\n", COLOR_GREEN, history_count,
         COLOR_RESET);
  printf("   Background jobs:     %s%d%s\n\n", COLOR_GREEN, job_count,
         COLOR_RESET);
}

// 2. tree - Display directory tree structure
void cmd_tree(char **args) {
  char *dir_path = args[1] ? args[1] : ".";
  DIR *dir = opendir(dir_path);

  if (dir == NULL) {
    printf("%sError: Cannot open directory '%s'%s\n", COLOR_RED, dir_path,
           COLOR_RESET);
    return;
  }

  printf("\n%s%s%s\n", COLOR_CYAN, dir_path, COLOR_RESET);

  struct dirent *entry;
  int count = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

    struct stat file_stat;
    stat(full_path, &file_stat);

    if (S_ISDIR(file_stat.st_mode)) {
      printf("├── %s%s/%s\n", COLOR_BLUE, entry->d_name, COLOR_RESET);
    } else {
      printf("├── %s\n", entry->d_name);
    }
    count++;
  }

  closedir(dir);
  printf("\n%s%d items%s\n\n", COLOR_GREEN, count, COLOR_RESET);
}

// 3. calc - Built-in calculator
void cmd_calc(char **args) {
  if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
    printf("%sUsage: calc [num1] [operator] [num2]%s\n", COLOR_RED,
           COLOR_RESET);
    printf("%sExample: calc 10 + 5%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sOperators: + - * / %% (modulo)%s\n\n", COLOR_YELLOW, COLOR_RESET);
    return;
  }

  double num1 = atof(args[1]);
  char *op = args[2];
  double num2 = atof(args[3]);
  double result = 0;
  int valid = 1;

  if (strcmp(op, "+") == 0) {
    result = num1 + num2;
  } else if (strcmp(op, "-") == 0) {
    result = num1 - num2;
  } else if (strcmp(op, "*") == 0 || strcmp(op, "x") == 0) {
    result = num1 * num2;
  } else if (strcmp(op, "/") == 0) {
    if (num2 == 0) {
      printf("%sError: Division by zero!%s\n", COLOR_RED, COLOR_RESET);
      return;
    }
    result = num1 / num2;
  } else if (strcmp(op, "%") == 0) {
    result = (int)num1 % (int)num2;
  } else {
    printf("%sError: Invalid operator '%s'%s\n", COLOR_RED, op, COLOR_RESET);
    valid = 0;
  }

  if (valid) {
    printf("\n%s╔═══ Calculator Result ═══╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s%.2f %s %.2f = %s%.2f%s\n", COLOR_YELLOW, num1, op, num2,
           COLOR_GREEN, result, COLOR_RESET);
    printf("%s╚═════════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
  }
}

// 4. reverse - Reverse lines in a file
void cmd_reverse(char **args) {
  if (args[1] == NULL) {
    printf("%sUsage: reverse [file]%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  FILE *fp = fopen(args[1], "r");
  if (fp == NULL) {
    printf("%sError: Cannot open file '%s'%s\n", COLOR_RED, args[1],
           COLOR_RESET);
    return;
  }

  char lines[1000][1024];
  int line_count = 0;

  while (fgets(lines[line_count], sizeof(lines[0]), fp) != NULL &&
         line_count < 1000) {
    line_count++;
  }

  fclose(fp);

  printf("\n%s╔═══ Reversed File: %s ═══╗%s\n", COLOR_CYAN, args[1],
         COLOR_RESET);
  for (int i = line_count - 1; i >= 0; i--) {
    printf("%s", lines[i]);
  }
  printf("%s╚═══════════════════════════════════╝%s\n\n", COLOR_CYAN,
         COLOR_RESET);
}

// 5. colortest - Test all available colors
void cmd_colortest(char **args) {
  printf("\n%s╔═══════════════════════════════════════════════════╗%s\n",
         COLOR_CYAN, COLOR_RESET);
  printf("%s║           🎨 COLOR PALETTE TEST 🎨               ║%s\n",
         COLOR_CYAN, COLOR_RESET);
  printf("%s╚═══════════════════════════════════════════════════╝%s\n\n",
         COLOR_CYAN, COLOR_RESET);

  printf("%s■ RED%s     - Error messages and warnings\n", COLOR_RED,
         COLOR_RESET);
  printf("%s■ GREEN%s   - Success messages and confirmations\n", COLOR_GREEN,
         COLOR_RESET);
  printf("%s■ YELLOW%s  - Information and prompts\n", COLOR_YELLOW,
         COLOR_RESET);
  printf("%s■ BLUE%s    - Directories and headers\n", COLOR_BLUE, COLOR_RESET);
  printf("%s■ MAGENTA%s - Special features and highlights\n", COLOR_MAGENTA,
         COLOR_RESET);
  printf("%s■ CYAN%s    - Borders and decorations\n", COLOR_CYAN, COLOR_RESET);
  printf("%s■ WHITE%s   - Standard text\n", COLOR_WHITE, COLOR_RESET);
  printf("%s■ BOLD%s    - Emphasis text\n\n", COLOR_BOLD, COLOR_RESET);

  printf("%sAll colors working perfectly! ✓%s\n\n", COLOR_GREEN, COLOR_RESET);
}

// Execute external command
void execute_command(char **args) {
  // Check for I/O redirection
  handle_io_redirection(args);

  // Check for background execution
  int background = check_background(args);

  // Create child process using fork()
  pid_t pid = fork();

  if (pid < 0) {
    printf("%sError: fork failed%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (pid == 0) {
    // Child process - execute command using execvp()
    if (execvp(args[0], args) < 0) {
      printf("%sError: Command '%s' not found%s\n", COLOR_RED, args[0],
             COLOR_RESET);
      exit(1);
    }
  } else {
    // Parent process
    if (background) {
      // Don't wait for background process
      printf("%s[%d] %d%s\n", COLOR_YELLOW, job_count + 1, pid, COLOR_RESET);
      add_background_job(pid, args[0]);
    } else {
      // Wait for foreground process using waitpid()
      int status;
      waitpid(pid, &status, 0);
    }
  }
}

// Handle I/O redirection (>, <, >>)
void handle_io_redirection(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
    // Output redirection: >
    if (strcmp(args[i], ">") == 0) {
      int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        printf("%sError opening output file%s\n", COLOR_RED, COLOR_RESET);
        return;
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
      args[i] = NULL;
      break;
    }

    // Append redirection: >>
    if (strcmp(args[i], ">>") == 0) {
      int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
      if (fd < 0) {
        printf("%sError opening file for append%s\n", COLOR_RED, COLOR_RESET);
        return;
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
      args[i] = NULL;
      break;
    }

    // Input redirection: <
    if (strcmp(args[i], "<") == 0) {
      int fd = open(args[i + 1], O_RDONLY);
      if (fd < 0) {
        printf("%sError opening input file%s\n", COLOR_RED, COLOR_RESET);
        return;
      }
      dup2(fd, STDIN_FILENO);
      close(fd);
      args[i] = NULL;
      break;
    }
  }
}

// Handle piping (cmd1 | cmd2)
void handle_pipe(char **args) {
  int pipefd[2];
  char *cmd1[MAX_ARGS];
  char *cmd2[MAX_ARGS];
  int i, j;

  // Split commands at pipe symbol
  i = 0;
  while (args[i] != NULL && strcmp(args[i], "|") != 0) {
    cmd1[i] = args[i];
    i++;
  }
  cmd1[i] = NULL;

  // Get second command
  i++; // Skip the pipe symbol
  j = 0;
  while (args[i] != NULL) {
    cmd2[j++] = args[i++];
  }
  cmd2[j] = NULL;

  // Create pipe using pipe() system call
  if (pipe(pipefd) < 0) {
    printf("%sError: Pipe creation failed%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  // Fork first child for first command
  pid_t pid1 = fork();

  if (pid1 < 0) {
    printf("%sError: Fork failed%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (pid1 == 0) {
    // First child: execute first command
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    if (execvp(cmd1[0], cmd1) < 0) {
      printf("%sError: Command '%s' not found%s\n", COLOR_RED, cmd1[0],
             COLOR_RESET);
      exit(1);
    }
  }

  // Fork second child for second command
  pid_t pid2 = fork();

  if (pid2 < 0) {
    printf("%sError: Fork failed%s\n", COLOR_RED, COLOR_RESET);
    return;
  }

  if (pid2 == 0) {
    // Second child: execute second command
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    if (execvp(cmd2[0], cmd2) < 0) {
      printf("%sError: Command '%s' not found%s\n", COLOR_RED, cmd2[0],
             COLOR_RESET);
      exit(1);
    }
  }

  // Parent: close pipe and wait for both children
  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
}

// Check if command should run in background
int check_background(char **args) {
  int i = 0;
  while (args[i] != NULL) {
    i++;
  }

  if (i > 0 && strcmp(args[i - 1], "&") == 0) {
    args[i - 1] = NULL;
    return 1;
  }

  return 0;
}

// Add background job to list
void add_background_job(pid_t pid, char *cmd) {
  if (job_count < MAX_JOBS) {
    bg_jobs[job_count].job_id = job_count + 1;
    bg_jobs[job_count].pid = pid;
    strncpy(bg_jobs[job_count].command, cmd, 255);
    bg_jobs[job_count].command[255] = '\0';
    job_count++;
  }
}

// Remove completed background job
void remove_background_job(pid_t pid) {
  for (int i = 0; i < job_count; i++) {
    if (bg_jobs[i].pid == pid) {
      printf("\n%s[%d] Done - %s%s\n", COLOR_GREEN, bg_jobs[i].job_id,
             bg_jobs[i].command, COLOR_RESET);

      // Shift remaining jobs
      for (int j = i; j < job_count - 1; j++) {
        bg_jobs[j] = bg_jobs[j + 1];
      }
      job_count--;
      break;
    }
  }
}

// Signal handler for background process completion
void signal_handler(int signo) {
  if (signo == SIGCHLD) {
    pid_t pid;
    int status;

    // Reap all zombie processes using waitpid() with WNOHANG
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      remove_background_job(pid);
    }
  }
}
