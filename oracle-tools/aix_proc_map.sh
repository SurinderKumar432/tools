
PID=$1

# to see mmap and shmat
svmon -P $PID  -O mapping=on,shmid=on,filename=on

# also /proc/<pid>/map is in binary format on AIX : decrypt using tool

# shows only loaded modules like libraries.
# (dbx) map 
# following works only on core file : all | shm | mmap etc
# (dbx) coremap shm
# (dbx) coremap mmap
