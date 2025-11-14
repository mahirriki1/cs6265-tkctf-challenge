#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "flag.h"

void start() {
  char buf[0x100];
  scanf("%s", buf);
  return;
}

int main(int argc, char *argv[], char *envp[])
{
  setreuid(geteuid(), geteuid());

  printf("BOF Level 0x00\n");
  /* strip env */
  char **env = envp;
  while (*env != NULL) {
    char *delim = strchr(*env, '=');

    *delim = '\0';
    printf("Discarding: %s\n", *env);
    *delim = '=';

    char *end = *env + strlen(*env);
    for (char *iter = *env; iter < end; iter ++) {
      *iter = '\0';
    }

    env ++;
  }

  start();
}

