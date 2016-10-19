#!/bin/sh

# Author: Surinder

if test $# -lt 2
then
   echo "usage: $0 length file1 file2 ..."
   echo "usage: $0 80 hello.c"
   exit 1
fi

length=$1

shift

LONGLINE=/tmp/longest-line-$$.awk

cat << EOF > $LONGLINE
  BEGIN {
  }

  /.*/ {
    current_length=length(\$0);
    if (current_length >= expected_length) {
       printf("%d at line # %d %s\n", current_length, NR, \$0);
    }
  }

  END {
  }
EOF

for file in $*
do
  echo "$file"
  cat $file | awk -v expected_length=$length -f $LONGLINE |sort -nr
done

rm $LONGLINE
