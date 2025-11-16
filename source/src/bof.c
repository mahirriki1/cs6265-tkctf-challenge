// vuln.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "flag.h"

/*
 * Manual stack frame layout:
 *   struct frame {
 *     char buf[0x100];
 *     uint32_t canary;   <- our manual canary
 *   };
 *
 * We place the canary right after the buffer in the stack frame so any overflow
 * that tries to smash the return address must overwrite the canary first.
 *
 * We also print a small info leak (buffer address and the canary) to force the
 * attacker to first leak the canary and then craft a payload that includes it.
 *
 * This increases difficulty (can't just overflow straight to ret) but preserves
 * a path to exploit (leak -> exploit).
 */

static uint32_t global_canary = 0;

static void init_canary(void)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        /* fallback */
        global_canary = (uint32_t) (getpid() ^ 0xdeadbeef);
        return;
    }
    uint32_t v;
    ssize_t r = read(fd, &v, sizeof(v));
    if (r != sizeof(v))
        v = (uint32_t) (getpid() ^ 0xcafebabe);
    close(fd);

    /* clear low byte to avoid accidental zero-termination issues when printing */
    /* (optional — keeps it simple for exploitation) */
    // v &= 0xFFFFFF00;

    global_canary = v;
}

void start() {
    /* put buffer first then the canary so the canary sits directly after the buffer */
    struct {
        char buf[0x100];
        uint32_t canary;
    } frame;

    /* set up our canary in the stack frame */
    frame.canary = global_canary;

    /* Intentionally vulnerable input (no length limit) - CTF style */
    /* Read a whitespace-terminated token from stdin into frame.buf */
    /* Keep the unsafe scanf("%s", ...) so overflow is still possible */
    /* but will be caught by our manual canary check unless the attacker supplies the correct canary bytes */
    printf("Buffer is at %p\n", (void*)frame.buf);
    printf("Canary (hex): 0x%08x\n", global_canary); /* small leak to make challenge solvable */
    fflush(stdout);

    scanf("%s", frame.buf);

    /* Echo-like behavior to mimic original program */
    if (!strcmp(frame.buf, "250382"))
        printf("Password OK :)\n");
    else
        printf("Invalid Password!\n");

    /* before returning, check the canary */
    if (frame.canary != global_canary) {
        /* detected smash */
        errx(1, "Stack smashing detected (manual canary)!");
    }

    return;
}

int main(int argc, char *argv[], char *envp[])
{
    /* the binary will be set-UID or similar in some CTF setups; preserve behavior */
    setreuid(geteuid(), geteuid());

    /* strip env */
    char **env = envp;
    while (*env != NULL) {
        char *delim = strchr(*env, '=');
        if (!delim) { env++; continue; }
        *delim = '\0';
        *delim = '=';

        char *end = *env + strlen(*env);
        for (char *iter = *env; iter < end; iter ++) {
            *iter = '\0';
        }

        env ++;
    }

    init_canary();
    start();
    return 0;
}
