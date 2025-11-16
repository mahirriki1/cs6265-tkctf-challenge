#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "flag.h"

void start() {
  char buf[0x100];
  /* Read a whitespace-terminated token from stdin into buf */
  scanf("%s", buf);

  if (!strcmp(buf, "250382"))
    printf("Password OK :)\n");
  else
    printf("Invalid Password!\n");

  return;
}

int main(int argc, char *argv[], char *envp[])
{
  setreuid(geteuid(), geteuid());

  /* strip env */
  char **env = envp;
  while (*env != NULL) {
    char *delim = strchr(*env, '=');

    *delim = '\0';
    *delim = '=';

    char *end = *env + strlen(*env);
    for (char *iter = *env; iter < end; iter ++) {
      *iter = '\0';
    }

    env ++;
  }

  start();
}

