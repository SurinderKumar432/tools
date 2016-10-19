
#!/bin/sh

# using lscpu
lscpu | grep -e 'Byte Order'

# using gdb
GDB_INPUT=/tmp/endian-$$.gdb

cat << EOF > $GDB_INPUT
show endian
quit
EOF

gdb -q -command=$GDB_INPUT

rm $GDB_INPUT
