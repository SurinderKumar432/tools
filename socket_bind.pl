#!/bin/perl

use English;
use Socket;
use IO::Socket;
use Sys::Hostname;

if (scalar @ARGV < 2)
{
  print "usage: $0 <host> <port>";
  exit 1;
}

$host = $ARGV[0] ;

if ($host eq "hostname")
{
  $host = hostname();
}
elsif ($host eq "any")
{
  $host = "0.0.0.0";
}
elsif ($host eq "loopback")
{
  $host = "127.0.0.1";
}

$port = $ARGV[1] ;

print "$host:$port\n";

# my $socket = IO::Socket::INET->new(Proto => "tcp", LocalPort=> $port);
my $socket = IO::Socket::INET->new(Proto => "tcp", LocalAddr => $host, LocalPort=> $port)  or die "Cannot create socket: $@";

my $mysockaddr = getsockname($socket);
my ($port, $myaddr) = sockaddr_in($mysockaddr);

printf "Connect to %s [%s] %s\n",
       scalar gethostbyaddr($myaddr, AF_INET),
       inet_ntoa($myaddr), $port;

close($socket);
