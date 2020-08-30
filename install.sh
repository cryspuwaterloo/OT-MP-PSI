# Install scripts for fast deployment to AWS

sudo apt-get update && sudo apt-get install make cmake gcc g++ m4 libgmp-dev

# GMP 6.1.2
wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
tar -jxvf gmp-6.1.2.tar.bz2 gmp-6.1.2/
cd gmp-6.1.2/
./configure --prefix=/usr --enable-cxx
make && sudo make install
cd ..
rm -rf gmp-6.1.2 && rm gmp-6.1.2.tar.bz2 


# NTL 11.4.1
wget https://www.shoup.net/ntl/ntl-11.4.1.tar.gz
tar xf ntl-11.4.1.tar.gz 
cd ntl-11.4.1/src/
./configure 
make && sudo make install 
cd ../..
rm -rf ntl-11.4.1 && rm ntl-11.4.1.tar.gz 

# libhcs
git clone https://github.com/tiehuis/libhcs
cd libhcs
cmake .
make
sudo make install

# Export install paths 
export LD_LIBRARY_PATH="/usr/local/lib"
