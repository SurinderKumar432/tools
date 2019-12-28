
netstat -in

# Sample output : for MTU on AIX
# 
# $netstat -in
# Name  Mtu   Network     Address           Ipkts Ierrs    Opkts Oerrs  Coll
# en0   1500  link#2      a.b.c.d.e.f 971660335     0 128087189     0     0
# en0   1500  Y.Y.Y.Y   X.X.X.X    971660335     0 128087189     0     0
# lo0   16896 link#1                        4737253     0  4737272     0     0
# lo0   16896 127         127.0.0.1         4737253     0  4737272     0     0
# lo0   16896 ::1%1                         4737253     0  4737272     0     0
# $
