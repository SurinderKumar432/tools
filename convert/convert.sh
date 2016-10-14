#!/bin/sh

# Author: Surinder

if test $# -ne 3
then
   echo "usage: $0 dec hex 20"
   echo "usage: $0 hex dec F"
   exit 1
fi

IBASE=$1
OBASE=$2
NUMBER=$3

case "$IBASE" in
   "dec" )
            IBASE=10
            ;;
   "hex" )
            IBASE=16
            ;;
esac


case "$OBASE" in
   "dec" )
            OBASE=10
            ;;
   "hex" )
            OBASE=16
            ;;
esac


# NOTE: keep obase before ibase otherwise obase number will be misinterepreted
cat << END  > convert.bc
obase=$OBASE
ibase=$IBASE
print $NUMBER
print "\n"
quit
END

bc -q convert.bc
