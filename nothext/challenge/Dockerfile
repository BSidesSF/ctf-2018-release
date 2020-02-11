FROM php:7.0-apache

# Upload the PHP stuff
ADD php_src/*.php /var/www/html/
RUN chmod -R ugo+rX /var/www/html/

EXPOSE 80
