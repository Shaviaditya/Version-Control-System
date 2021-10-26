#!/bin/bash

# to get the hashes via openssl
sudo apt-get install openssl -y
sudo apt-get install libssl-dev -y
#making the directory to work with it
mkdir -p ~/imperium/bin

#Copy install script to absolute
cp imperium.sh ~/imperium
cd ..

#Compile & save our cpp file
make
cd ~/imperium/bin || echo "error"

# Making executable file
chmod +x main
cd ..
# Compiling 2 files to check whether its already installed
if grep -q "source $PWD/imperium.sh" "$PWD/../.bashrc" ; then
    echo 'already installed bash source';
else 
    echo "source $PWD/imperium.sh" >> ~/.bashrc;
fi
