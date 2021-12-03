#!/bin/bash

# to get the hashes via openssl
sudo apt-get install openssl -y
sudo apt-get install libssl-dev -y
#making the directory to work with it
mkdir -p ~/kronos/bin

#Copy install script to absolute
cp kronos.sh ~/kronos
cd ..

#Compile & save our cpp file
make
cd ~/kronos/bin || echo "error"

# Making executable file
chmod +x main
cd ..
# Compiling 2 files to check whether its already installed
if grep -q "source $PWD/kronos.sh" "$PWD/../.bashrc" ; then
    echo 'already installed bash source';
else 
    echo "source $PWD/kronos.sh" >> ~/.bashrc;
fi
