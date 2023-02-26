#!/bin/bash

if [ ! $# -eq 1 ] 
then
    echo "Error: extension is empty!"
    exit 1
fi
LOG=`pwd`
LOG+="out2.log"
until [ "`pwd`" = "/" ]
do
    DIR=`pwd`
    echo -e "\nDIRECTORY: "$DIR"\n"
    ls | grep *$1
    cd ..
done

cd ..
DIR=`pwd`
echo -e "\nDIRECTORY: "$DIR"\n"
ls | grep *$1