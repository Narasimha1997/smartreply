FROM ubuntu:18.04

RUN apt-get update && \
  apt-get install -y software-properties-common 


RUN apt-get install -y build-essential python3.6 python3.6-dev python3-pip python3.6-venv cmake
RUN apt-get install -y git

# update pip
RUN python3.6 -m pip install pip --upgrade
RUN python3.6 -m pip install wheel
RUN python3.6 -m pip install flask
RUN mkdir workspace

COPY . /workspace

WORKDIR /workspace

ENTRYPOINT ["python3", "server.py"]
