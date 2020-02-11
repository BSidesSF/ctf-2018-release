FROM debian:stretch-slim

ENV DEBIAN_FRONTEND noninteractive
ENV ARM_HOST arm.challenges.bsidessf.net:6432

RUN apt-get update && \
  apt-get install -y xinetd && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/*

RUN useradd -m -k $(mktemp -d) coder

COPY service /home/coder/service
COPY coder.xinetd /etc/xinetd.d/coder
COPY flag.txt /home/coder/flag.txt
COPY coder.sh /home/coder/coder.sh

RUN chown coder:coder \
  /home/coder/service \
  /home/coder/flag.txt \
  /home/coder/coder.sh

EXPOSE 6432/tcp

ENTRYPOINT ["xinetd", "-dontfork"]
