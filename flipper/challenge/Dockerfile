FROM debian:stretch-slim

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
  apt-get install -y python python-pip nginx gunicorn supervisor procps && \
  apt-get clean && \
  useradd -r -M flipper && \
  mkdir -p /srv

COPY . /srv
RUN pip install -r /srv/requirements.txt && \
  chown -R flipper:flipper /srv && \
  chmod -R ugo+rX-w /srv && \
  chmod u+w /srv/data

RUN rm /etc/nginx/sites-enabled/default
COPY conf/nginx.conf /etc/nginx/sites-available/flask.conf
RUN ln -s /etc/nginx/sites-available/flask.conf \
  /etc/nginx/sites-enabled/flask.conf && \
  echo "daemon off;" >> /etc/nginx/nginx.conf

RUN mkdir -p /var/log/supervisor
COPY conf/supervisord.conf /etc/supervisor/conf.d/supervisord.conf

CMD ["/usr/bin/supervisord"]
