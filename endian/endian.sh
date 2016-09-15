
#!/bin/sh

# using lscpu
lscpu | grep -e 'Byte Order'

# using gdb
echo "show endian" > /tmp/gdb.endian
echo "quit" >> /tmp/gdb.endian
gdb -q -command=/tmp/gdb.endian
