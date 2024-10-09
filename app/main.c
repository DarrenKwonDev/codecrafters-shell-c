
#include <stdio.h>
#include <string.h>

int main() {
  setbuf(stdin, NULL); setbuf(stdout, NULL);
  printf("$ ");
  fflush(stdout); 

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin); // stdin에서 input에 100 char 읽어옴
  input[strlen(input) - 1] = '\0'; // ex - strlen(asdf\n) 은 5 반환. \n를 \0로 변경하기 위해서 4 번째 자리에 \0 넣어야.

  fprintf(stdout, "%s: command not found\n", input);
  
  return 0;
}
