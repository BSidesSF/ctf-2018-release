FROM php:7.0-apache
RUN apt-get update && apt-get install -y \
        libjpeg62-turbo-dev \
        libpng12-dev \
    && docker-php-ext-configure gd --with-jpeg-dir=/usr/include/ \
      --with-png-dir=/usr/include \
    && docker-php-ext-install -j$(nproc) gd \
    && docker-php-ext-enable gd

ADD index.php /var/www/html/
ADD images/* /tmp/images/
RUN chmod -R ugo+rX /var/www/html/
RUN chmod 0777 /tmp/images
RUN chmod -R ugo+rX /tmp/images/

EXPOSE 80
