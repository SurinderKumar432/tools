#!/bin/sh

# Author: Surinder
# Hexadecimal to Decimal converter
# Decimal to Hexadecimal convert

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
CONV_FILE=convert_$$.bc
cat << END  > $CONV_FILE
obase=$OBASE
ibase=$IBASE
print $NUMBER
print "\n"
quit
END

bc -q $CONV_FILE

rm $CONV_FILE
