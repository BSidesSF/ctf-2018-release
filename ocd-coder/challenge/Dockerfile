FROM debian:stretch-slim

ENV DEBIAN_FRONTEND noninteractive

RUN dpkg --add-architecture i386 && \
  apt-get update && \
  apt-get install -y xinetd libc6:i386 && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/*

RUN useradd -m -k $(mktemp -d) coder

COPY coder /home/coder/coder
COPY coder.xinetd /etc/xinetd.d/coder
COPY flag.txt /home/coder/flag.txt
COPY coder.sh /home/coder/coder.sh

RUN chown coder:coder \
  /home/coder/coder \
  /home/coder/flag.txt \
  /home/coder/coder.sh

EXPOSE 9999

ENTRYPOINT ["xinetd", "-dontfork"]
