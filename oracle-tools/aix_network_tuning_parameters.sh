
no -a | grep -e ipqmaxlen -e sb_max -e rfc1323  -e sendspace -e recvspace

# Expected output
#                ipqmaxlen = 512
#                  rfc1323 = 1
#                   sb_max = 4194304
#            tcp_recvspace = 65536
#            tcp_sendspace = 65536
#            udp_recvspace = 655360
#            udp_sendspace = 65536
