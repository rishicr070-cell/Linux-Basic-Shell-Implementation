#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char *line = NULL; size_t len = 0; ssize_t read;
    
    while (1) {
        printf("myshell> ");
        read = getline(&line, &len, stdin);
        if (read == -1) break;
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "exit") == 0) break;
        if (strncmp(line, "cd ", 3) == 0) {
            chdir(line + 3); continue;
        }
        
        char *args[64]; int i = 0;
        char *token = strtok(line, " ");
        while (token && i < 63) { args[i++] = token; token = strtok(NULL, " "); }
        args[i] = NULL;
        
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("Command not found"); exit(1);
        } else { wait(NULL); }
    }
    free(line); return 0;
}
