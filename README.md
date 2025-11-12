# TKCTF

This is a template for your docker image that will be used to host
your CTF challenge.

## Structure

~~~~{.sh}
    /NAME                : team-name:challenge-name (e.g., `staff:fmstr`)
    /PORT                : a port number

    # all files to run your service
    /docker/Dockerfile   : Dockerfile
           /target       : target bin
           /flag         : flag: tkctf{please submit this flag!}

    # all files to be released to participants
    /release/README.md   : guideline
            /target      : bin if you want to release

    # source/exploit for your team and organizer
    /source/test.sh      : build docker, run, run exploit.py and print out flag
           /exploit.py   : exploit
           /writeup.txt  : solution
           /src/         : source code
~~~~

1. Change the `NAME` file to your challenge name: `teamname:challenge`
   (e.g., `staff:fmtstr`)
2. Change the `docker/flag` file as you wish, but wrap it inside `tkctf{...}`
   (e.g., `tkctf{please submit this flag!}`)

The source code of the challenge locates in `source/src/`, which
contains its source (`fmtstr.c`) and makefile (`Makefile`).
The makefile includes various defense options you can enable
(e.g., `CFLAGS += -fstack-protector`). Please carefully enable them
as you'd like for your challenge.

## Overview

~~~~{.sh}
    $ wget https://tc.gts3.org/cs6265/2025/_static/ctf-template.zip
    $ unzip ctf-template.zip

    $ cd ctf-template
    $ make help
    dist  : build the target and distribute to docker/release
    build : build the docker image
    run   : run the docker container
    test  : test the exploit
    submit: zip for submission
~~~~

Be imaginative but keep in mind that, the best challenge should be fun
and educational. Please don't design the challenge that requires any
random/pure guessing (but well-designed bruteforcing is fine).

You can replicate real security vulnerabilities, like `mini-sudo` or
`mini-heartbleed`. There are a few sources that you can refer:

- <https://ctftime.org/>
- <https://www.cvedetails.com/>
- <https://www.exploit-db.com/>

## Checklist for submission

1) /NAME: Team/challenge name
2) /release/README.md: Description about the challenge
3) /docker/flag: Flag!
4) /source/writeup.txt: Your description on the challenge and solution
5) /source/exploit.py: Your _working_ exploit
6) Triple check `make test` reliably executes!

Please `make submit`, upload it to a drive, and share the link for your file (e.g., `staff:fmtstr.zip`)
to [here](https://forms.office.com/r/k2fZRWpBDL)
by Nov 15. We will test, evaluate and provide some feedbacks
before the ctf event.
