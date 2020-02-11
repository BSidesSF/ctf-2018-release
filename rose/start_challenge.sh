#!/usr/bin/env bash
docker run -i --add-host secretshost.internal:127.0.0.1 -p 3000:3000 --rm --cap-add=SYS_ADMIN --name puppeteer-chrome puppeteer-chrome-linux node -e "`cat puppeteer-controller.js`"