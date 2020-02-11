FROM resin/armv7hf-debian-qemu

RUN [ "cross-build-start" ]

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
  apt-get install -y xinetd && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  ln -s /lib/ld-linux-armhf.so.3 /lib/ld-linux.so.3

RUN useradd -m -k $(mktemp -d) coder

COPY coder /home/coder/coder
COPY coder.xinetd /etc/xinetd.d/coder
COPY flag.txt /home/coder/flag.txt
COPY coder.sh /home/coder/coder.sh

RUN chown coder:coder \
  /home/coder/coder \
  /home/coder/flag.txt \
  /home/coder/coder.sh

RUN [ "cross-build-end" ]

EXPOSE 1978/tcp

ENTRYPOINT ["xinetd", "-dontfork"]
