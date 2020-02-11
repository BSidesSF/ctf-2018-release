FROM ubuntu:xenial

RUN adduser --disabled-password --gecos '' ctf

RUN apt-get update && apt-get install -y xinetd gcc make libgmp-dev

WORKDIR /home/ctf
ADD src/* /home/ctf/
#RUN make
RUN chown -R root:root .

ADD signit.xinetd /etc/xinetd.d/signit

USER root
EXPOSE 6112

CMD service xinetd restart && sleep infinity
