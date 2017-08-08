#!/bin/sh

# author : surinder

if [ $# -lt 2 ]; then
        echo "usage: $0 <libs_directory> <symbol> <nm_options>"
        echo "e.g: $0 /usr/lib/ pthread_create --defined-only"
        exit;
fi

DIR=$1
SYMBOL=$2

shift
shift

OPTIONS=$*

cd $DIR

for file in `ls`
do
        nm $OPTIONS $file 2>/dev/null | grep $SYMBOL
        if [ $? -eq 0 ]; then
                echo $file
        fi
done

