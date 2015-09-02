#!/bin/sh

if [ $# -lt 1 ]
then
  echo "usage: $0 <errno>"
  exit 1 
fi

errno=$1

# will include other header files as well
cpp -dM /usr/include/errno.h | grep -w "$errno" | grep -e 'define E'

perl -MPOSIX -e 'print "$ARGV[0] : ". strerror($ARGV[0])."\n";' $errno

# /usr/bin/perror : in mysql-server package
perror $errno
