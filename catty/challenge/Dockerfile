#
# Bsides San Fran Netcat Challenge Docker Image
#
# Created By Christopher Davis
# BUILD / RUN / SSH / CHALLENGE SOLUTION - INSTRUCTIONS:
#
# $ sudo docker build -t "bsides_netcat_challenge" .
# $ sudo docker run -p 5000:22 --name bsides_netcat.test1 -t bsides_netcat_challenge &
# $ ssh bsides@localhost -p 5000
# $ export SESSIONID="$(printf $RANDOM$RANDOM)"
# $ ./launch_nc.elf
# $ nc -vl -p 9262 & echo "HELLO" | nc -buw3 -s 127.41.11.206 -p 6354 127.255.255.255 16734
#
# SEE ALSO solution.png for screenshot of solution

# Pull base image.
FROM ubuntu

# Update and Install Packages.
RUN apt-get  update && \
  apt-get -y upgrade && \
  apt-get install -y man \
  netcat-traditional \
  expect \
  adduser \
  openssh-server \
  sudo \
  sed \
  openssl \
  python3 \
  expect

RUN mkdir /var/run/sshd
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile
EXPOSE 22

# install files.
COPY root/adduser_bsides.sh /root/adduser_bsides.sh
COPY root/adduser_netcat.sh /root/adduser_netcat.sh
COPY root/motd /etc/motd
COPY root/netcat.sh /home/netcat/netcat.sh
COPY root/flag.txt /root/flag.txt
COPY root/nc_starter.py /root/nc_starter.py

RUN chmod 1700 /root/nc_starter.py

RUN chmod 700 /root/adduser*
RUN /root/adduser_netcat.sh
RUN chmod 700 /home/netcat/netcat.sh
RUN chown netcat:netcat /home/netcat/netcat.sh
RUN chmod 700 /root/flag.txt
RUN chown netcat:netcat /root/flag.txt

RUN /root/adduser_bsides.sh

RUN echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config

#RUN mkdir /home/bsides
RUN touch /home/bsides/.bash_history
RUN chown bsides:bsides /home/bsides

RUN usermod --password $(echo bsides | openssl passwd -1 -stdin) bsides
RUN usermod --password $(echo Totes_Difficult_Password1927 | openssl passwd -1 -stdin) netcat

RUN mkdir -p /home/bsides/dev/
RUN mknod -m 666 /home/bsides/dev/null c 1 3
RUN mknod -m 666 /home/bsides/dev/tty c 5 0
RUN mknod -m 666 /home/bsides/dev/zero c 1 5
RUN mknod -m 666 /home/bsides/dev/random c 1 8

RUN chown root:root /home/bsides
RUN chmod 0755 /home/bsides -R

RUN mkdir /home/bsides/home
RUN mkdir /home/bsides/tmp

RUN chmod 1777 /home/bsides/home
RUN chmod 1777 /home/bsides/tmp

RUN usermod -d /home bsides

RUN mkdir -p /home/bsides/bin
RUN cp -v /bin/bash /home/bsides/bin/
RUN cp -v /bin/nc.traditional /home/bsides/bin/nc
RUN cp -v /bin/ls /home/bsides/bin/ls
RUN cp -v /bin/rm /home/bsides/bin/
RUN cp -v /bin/cat /home/bsides/bin/
RUN cp -v /usr/bin/tty /home/bsides/bin/
RUN cp -v /bin/sh /home/bsides/bin/
RUN cp -v /usr/bin/env /home/bsides/bin/
RUN cp -v /usr/bin/printf /home/bsides/bin/

RUN mkdir -p /home/bsides/lib/
RUN mkdir -p /home/bsides/lib/x86_64-linux-gnu
RUN mkdir -p /home/bsides/lib64
RUN cp -v /lib/x86_64-linux-gnu/libtinfo.so.5 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libdl.so.2 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libc.so.6 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libselinux.so.1 /home/bsides/lib/x86_64-linux-gnu/


RUN cp -v /lib/x86_64-linux-gnu/libpthread.so.0 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libselinux.so.1 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libutil.so.1 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libm.so.6 /home/bsides/lib/x86_64-linux-gnu/


RUN cp -v /lib/x86_64-linux-gnu/libpcre.so.3 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libexpat.so.1 /home/bsides/lib/x86_64-linux-gnu/
RUN cp -v /lib/x86_64-linux-gnu/libz.so.1 /home/bsides/lib/x86_64-linux-gnu/

RUN mkdir /home/bsides/dist

COPY root/dist /home/bsides/dist/
RUN chmod -R ugo+rX /home/bsides/dist

RUN cp -v /lib64/ld-linux-x86-64.so.2 /home/bsides/lib64/


RUN mkdir /home/bsides/etc
RUN cp -vf /etc/passwd /home/bsides/etc/
RUN cp -vf /etc/group /home/bsides/etc/

RUN echo "Match User bsides" >> /etc/ssh/sshd_config
RUN echo "ChrootDirectory /home/bsides" >> /etc/ssh/sshd_config

COPY root/motd /home/bsides/etc/motd

RUN touch /home/bsides/home/.bash_history

COPY root/make_link.sh /tmp
RUN chmod 755 /tmp/make_link.sh
RUN /tmp/make_link.sh
RUN rm -rf /tmp/make_link.sh

RUN chmod 1755 /home/bsides/home/launch_nc.elf
RUN chown root:root /home/bsides/home/launch_nc.elf

COPY bash_profile /home/bsides/home/.bash_profile
RUN chown root.root /home/bsides/home/.bash_profile
RUN chmod 0755 /home/bsides/home/.bash_profile

# Set environment variables.
ENV HOME /root

# Define working directory.
WORKDIR /root

# Define default command.
#CMD ["bash"]
CMD ["/root/nc_starter.py"]
