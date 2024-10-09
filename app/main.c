
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
  setbuf(stdin, NULL); 
  setbuf(stdout, NULL);
  char input[100];
  
  // REPL
  while (1)
  {
    memset(input, 0, sizeof(input));

    printf("$ ");
    fflush(stdout); 

    fgets(input, 100, stdin); // stdin에서 input에 100 char 읽어옴
    int input_len = strlen(input);
    input[input_len - 1] = '\0'; // ex - strlen(asdf\n) 은 5 반환. \n를 \0로 변경하기 위해서 4 번째 자리에 \0 넣어야.

    // exit
    if (strncmp(input, "exit", 4) == 0) {
      char* sep = strchr(input, ' ');
      int exit_num = atoi(sep + 1); // sep + 1 지점부터 문자열 끝까지를 정수화
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
      char buf[100];
      sprintf(buf, "%s", sep);
      if (strncmp(buf, "type", 4) == 0 || 
          strncmp(buf, "echo", 4) == 0 || 
          strncmp(buf, "exit", 4) == 0) {
          fprintf(stdout, "%s is a shell builtin\n", buf);
        } else {
          fprintf(stdout, "%s: not found\n", buf);
        }

      continue;
    }

    // not found
    fprintf(stdout, "%s: command not found\n", input);
  }

  return 0;
}
