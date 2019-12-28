#!/bin/sh

if test $# -ne 1
then
   echo "usage: $0 <exec | lib> path"
   exit 1
fi

# exec or lib name

EXEC_LIB=$1

readelf -d $EXEC_LIB  | grep RPATH
