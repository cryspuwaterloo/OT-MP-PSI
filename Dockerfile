FROM ubuntu:18.04
WORKDIR /usr/src/app
COPY . .

# preliminaries and git

RUN apt-get update && apt-get -y install curl
RUN apt-get install wget
RUN apt-get update && apt-get -y install make cmake gcc g++ m4 libgmp-dev
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN apt-get update && apt-get -y install git

# GMP 6.1.2
RUN wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
RUN tar -jxvf gmp-6.1.2.tar.bz2 gmp-6.1.2/
RUN cd gmp-6.1.2/ && ./configure --prefix=/usr --enable-cxx \
    && make && make install \
    && cd .. \
    && rm -rf gmp-6.1.2 && rm gmp-6.1.2.tar.bz2 


# NTL 11.4.1
RUN wget https://www.shoup.net/ntl/ntl-11.4.1.tar.gz
RUN tar xf ntl-11.4.1.tar.gz 
RUN cd ntl-11.4.1/src/ && ./configure \
    && make && make install \
    && cd ../..

RUN rm -rf ntl-11.4.1 && rm ntl-11.4.1.tar.gz

# libhcs
RUN git clone https://github.com/tiehuis/libhcs \
    && cd libhcs \
    && cmake . \
    && make \
    && make install

# Export install paths 
ENV LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"

#commands to run

RUN cd benchmark && make \
    && cd .. \
    && cd server && make 


#CMD cd Code/server && ./server \
#   && cd .. && cd benchmark && ./benchmark all -m 10 -n 100 -t 2 -b 2048 -k 127.0.0.1 -s 0

