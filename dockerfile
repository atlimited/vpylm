# image
FROM ubuntu:bionic

RUN apt-get update && apt-get install -y python3-dev libboost-all-dev git cmake g++ gdb python3-dbg libgoogle-glog-dev libgflags-dev wget clang mecab libmecab-dev mecab-ipadic-utf8 python3-pip

# language environment settings
RUN apt-get -y install language-pack-ja-base language-pack-ja

# env var
ENV LANG ja_JP.UTF-8
ENV CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python3.7m/"

# mecab
RUN apt-get -y install git mecab libmecab-dev mecab-ipadic-utf8 python3-pip
RUN pip3 install mecab-python3==0.996.5

# libboost
RUN apt-get install wget
RUN wget -O boost_1_75_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.75.0/boost_1_75_0.tar.gz/download && tar xzvf boost_1_75_0.tar.gz 
WORKDIR boost_1_75_0
RUN ./bootstrap.sh --prefix=/usr/
RUN ./b2 && ./b2 install

# make
WORKDIR /workspace/
RUN git clone https://github.com/atlimited/vpylm.git
WORKDIR vpylm
RUN python3 utils/process.py -t data/raw/ -s data/processed/
RUN make
