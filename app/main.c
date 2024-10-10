#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATH 1024
#define MAX_COMMAND 256
#define MAX_EXEC_NAME 256

int find_command(const char* command, char* result) {
    char* path = getenv("PATH");
    if (path == NULL) {
        return 0;
    }

    char* path_copy = strdup(path);
    char* dir       = strtok(path_copy, ":");

    while (dir != NULL) {
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        if (access(full_path, X_OK | F_OK) == 0) {
            strcpy(result, full_path);
            free(path_copy);
            return 1;
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

void fork_and_exec_cmd(char *full_path, int argc, char **argv) {
    pid_t pid = fork();
    if (pid == 0) {
        execv(full_path, argv);
        perror("execv");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    char input[100];
    char exec_name[MAX_EXEC_NAME];

    while (1) {
        memset(input, 0, sizeof(input));

        printf("$ ");
        fflush(stdout);

        fgets(input, 100, stdin);
        int input_len = strlen(input);
        input[input_len - 1] = '\0';

        if (strncmp(input, "exit", 4) == 0) {
            if (strcmp(input, "exit 0") == 0) {
                return 0;
            }
            continue;
        }

        if (strncmp(input, "echo", 4) == 0) {
            printf("%s\n", input + 5);
            continue;
        }

        if (strncmp(input, "type", 4) == 0) {
            char* command = input + 5;
            if (strcmp(command, "exit") == 0 ||
                strcmp(command, "echo") == 0 ||
                strcmp(command, "type") == 0) {
                printf("%s is a shell builtin\n", command);
            } else {
                char result[MAX_PATH];
                if (find_command(command, result)) {
                    printf("%s is %s\n", command, result);
                } else {
                    printf("%s: not found\n", command);
                }
            }
            continue;
        }

        char *argv[10];
        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL && argc < 10) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;

        if (find_command(argv[0], exec_name)) {
            fork_and_exec_cmd(exec_name, argc, argv);
        } else {
            printf("%s: command not found\n", argv[0]);
        }
    }

    return 0;
}