#!/bin/perl

use English;
use Socket;
use IO::Socket;
use Sys::Hostname;

if (scalar @ARGV < 3)
{
  print "usage: $0 <host> <port> <sleep_secs>\n";
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
elsif ($host eq "none")
{
  $host = "none"; 
}

$port = $ARGV[1] ;

$sleep = $ARGV[2] ;

print "$host:$port sleep $sleep\n";

my $socket;

if ($host eq "none")
{
  $socket = IO::Socket::INET->new(Proto => "tcp", LocalPort=> $port) or die "Cannot create socket: $@";
}
else
{
  $socket = IO::Socket::INET->new(Proto => "tcp", LocalAddr => $host, LocalPort=> $port)  or die "Cannot create socket: $@";
}

my $mysockaddr = getsockname($socket);
my ($port, $myaddr) = sockaddr_in($mysockaddr);

printf "Connect to %s [%s] %s\n",
       scalar gethostbyaddr($myaddr, AF_INET),
       inet_ntoa($myaddr), $port;

print "sleeping ...\n";

sleep $sleep;

print "done sleeping\n";

close($socket);
