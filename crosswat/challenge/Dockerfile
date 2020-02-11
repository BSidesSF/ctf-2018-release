FROM golang:1.10.0-alpine3.7
RUN apk add --no-cache \
    php5-cli=5.6.35-r0\
    git


ADD ./flag /flag
RUN addgroup -S ctf && adduser -S -g ctf ctf
WORKDIR /home/ctf

ADD src/ ./
RUN go get -d ./... && go build server.go && rm server.go


USER ctf
CMD ["./server"]
