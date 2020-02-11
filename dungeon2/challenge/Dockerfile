FROM bsidessf-ctf/nsjail

RUN apt-get update && apt-get install -y libncurses5-dev

RUN adduser --disabled-password --gecos '' ctf

WORKDIR /tmp
ADD src/ ./
ADD ./flag /flag
RUN make && mv level2 /home/ctf/ && rm -rf /tmp/*

# Add the nsjail configuration
ADD nsjail.conf /tmp/

# Run as an unprivileged user
WORKDIR /home/ctf/
USER ctf
CMD ["nsjail", "-C", "/tmp/nsjail.conf"]
