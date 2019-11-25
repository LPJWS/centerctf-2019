FROM ubuntu:16.04

COPY ./service /service
RUN apt-get update && \
    apt-get --assume-yes install socat && \ 
    chmod +x /service/on_the_edge
CMD socat TCP-LISTEN:1337,reuseaddr,fork EXEC:/service/on_the_edge