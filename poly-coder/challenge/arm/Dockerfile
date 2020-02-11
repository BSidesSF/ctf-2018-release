FROM resin/armv7hf-debian-qemu

RUN [ "cross-build-start" ]

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
  apt-get install -y xinetd && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  ln -s /lib/ld-linux-armhf.so.3 /lib/ld-linux.so.3

RUN useradd -m -k $(mktemp -d) coder

COPY service /home/coder/service
COPY coder.xinetd /etc/xinetd.d/coder
COPY coder.sh /home/coder/coder.sh

RUN chown coder:coder \
  /home/coder/service \
  /home/coder/coder.sh

RUN [ "cross-build-end" ]

EXPOSE 6432/tcp

ENTRYPOINT ["xinetd", "-dontfork"]
