FROM ubuntu:24.04

RUN useradd -m -s /bin/bash ctf

# enable 32-bit support
RUN dpkg --add-architecture i386
RUN apt update
RUN apt install -y libc6:i386 libstdc++6:i386

# install packages
RUN apt install -y socat

# copy target/flag files
COPY flag /flag
COPY target /target
COPY entry.sh /entry.sh

# make the flag readonly
RUN chmod 444 /flag
RUN chmod 755 /target
RUN chmod +x /entry.sh

# use ctf user instead of root
USER root

# run socat
ENTRYPOINT ["/entry.sh"]