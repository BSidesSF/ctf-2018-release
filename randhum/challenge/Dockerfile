FROM ubuntu:xenial

RUN adduser --disabled-password --gecos '' ctf

RUN apt-get update && apt-get install -y xinetd gcc make libc6-dev-i386 libssl-dev

WORKDIR /home/ctf
ADD src/* /home/ctf/
RUN make
RUN chown -R root:root .

ADD crontab /etc/cron.d/rerand
RUN chmod 0644 /etc/cron.d/rerand
RUN dd if=/dev/urandom of=/tmp/rngseed bs=4096 count=1

ADD randhum.xinetd /etc/xinetd.d/randhum

USER root
EXPOSE 2525

CMD service cron start && service xinetd restart && sleep infinity
