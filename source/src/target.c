#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

unsigned int canary = 0xdeadbeef;

void vulnerable() {
  char buf[0x100];
  unsigned int stored_canary = canary;
  
  printf("Enter password: ");
  fgets(buf, 512, stdin);
  
  if (stored_canary != canary) {
    printf("Stack smashing detected!\n");
    exit(-1);
  }
  
  printf("Password incorrect!\n");
}

int main(int argc, char *argv[], char *envp[])
{
  setreuid(geteuid(), geteuid());
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  /* strip env */
  char **env = envp;
  while (*env != NULL) {
    char *delim = strchr(*env, '=');
    if (delim) {
      *delim = '\0';
      *delim = '=';
    }
    char *end = *env + strlen(*env);
    for (char *iter = *env; iter < end; iter++) {
      *iter = '\0';
    }
    env++;
  }

  vulnerable();
  return 0;
}