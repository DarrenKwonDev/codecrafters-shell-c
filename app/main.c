
#include <stdio.h>
#include <string.h>

int main() {
  setbuf(stdin, NULL); setbuf(stdout, NULL);
  printf("$ ");
  fflush(stdout); 

  char input[100];
  fgets(input, 100, stdin); // stdin에서 input에 100 char 읽어옴
  input[strlen(input) - 1] = '\0'; // ex - strlen(asdf\n) = 5. 따라서 4번째 자리에 null char 를 포함해야 함

  fprintf(stdout, "%s: command not found\n", input);
  
  return 0;
}
