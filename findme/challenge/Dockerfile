FROM php:7.0-apache
#RUN apt-get update

COPY src/dist /var/www/html
RUN chown -R www-data:www-data /var/www/html

EXPOSE 80
