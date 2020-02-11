FROM node:carbon

WORKDIR /usr/src/app
COPY server /usr/src/app/
RUN npm install
RUN npm install -g sequelize-cli gulp
RUN sequelize db:migrate
RUN sequelize db:seed:all

EXPOSE 80

CMD /bin/bash -c "cd /usr/src/app && source dev.bash && gulp"
