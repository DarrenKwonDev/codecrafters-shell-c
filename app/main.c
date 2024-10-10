
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PATH 1024
#define MAX_COMMAND 256

int find_command(const char* command, char* result) {
    char* path = getenv("PATH");
    if (path == NULL) {
        return 0;
    }

    char* path_copy = strdup(path); // internal malloc. free 해야 함.
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

int main(int argc, char** argv) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    char input[100];

    // REPL
    while (1) {
        memset(input, 0, sizeof(input));

        printf("$ ");
        fflush(stdout);

        fgets(input, 100, stdin); // stdin에서 input에 100 char 읽어옴
        int input_len = strlen(input);
        input[input_len - 1] = '\0'; // ex - strlen(asdf\n) 은 5 반환. \n를 \0로 변경하기 위해서 4
                                     // 번째 자리에 \0 넣어야.

        // exit
        if (strncmp(input, "exit", 4) == 0) {
            char* sep      = strchr(input, ' ');
            int   exit_num = atoi(sep + 1); // sep + 1 지점부터 문자열 끝까지를 정수화
            return exit_num;
        }

        // echo
        if (strncmp(input, "echo", 4) == 0) {
            // echo hello world -> print hello world
            char* sep = strchr(input, ' ');
            sep++;
            printf("%s\n", sep);
            continue;
        }

        // type
        if (strncmp(input, "type", 4) == 0) {
            char* sep = strchr(input, ' ');
            sep++;
            char command[MAX_COMMAND];
            strcpy(command, sep);

            if (strncmp(command, "type", 4) == 0 || strncmp(command, "echo", 4) == 0 ||
                strncmp(command, "exit", 4) == 0) {
                fprintf(stdout, "%s is a shell builtin\n", command);
            } else {
                char result[MAX_PATH];
                if (find_command(command, result)) {
                    // command를 arguments와 함께 실행시키고 싶음.
                    // fork and exec 패턴
                    pid_t pid = fork();
                    if (pid == 0) { // child proc
                        execv(command, argv++);
                    } else { // parent proc
                        int status;
                        waitpid(pid, &status, 0);
                    }

                } else {
                    fprintf(stdout, "%s: not found\n", command);
                }
            }

            continue;
        }

        // not found
        fprintf(stdout, "%s: command not found\n", input);
    }

    return 0;
}
