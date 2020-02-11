FROM bsidessf-ctf/nsjail

RUN apt-get update && apt-get install -y python

# Add the nsjail configuration
ADD nsjail.conf /tmp/

# Set up the binary that gives you the flag
ADD src/flag.c /tmp/flag.c
RUN gcc -o /hooraay_run_me_to_get_your_flag /tmp/flag.c
RUN rm /tmp/flag.c
RUN chmod 111 /hooraay_run_me_to_get_your_flag

# Add the source code
ADD src/ /home/ctf
RUN adduser --disabled-password --gecos '' ctf

# Run as an unprivileged user
USER ctf
CMD ["nsjail", "-C", "/tmp/nsjail.conf"]
