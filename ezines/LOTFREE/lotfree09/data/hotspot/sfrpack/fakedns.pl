#!/usr/bin/perl
# Basic DNS server using Net::DNS::Nameserver. Answers all A record queries with 127.0.0.1
# Copyright (C) 2001  nop@0x90.co.uk - Geoff Jones

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use strict;
use warnings;
use Term::ANSIColor;
use Net::DNS::Nameserver;

# Reply handler to answer 127.0.0.1 to all requests
sub reply_handler {
	 my ($qname, $qclass, $qtype, $peerhost,$query,$conn) = @_;
	 my ($rcode, @ans, @auth, @add);

	 # print &dnsLabel . "Received query from $peerhost to ". $conn->{"sockhost"}. "\n";
	 # $query->print;

	 if ($qtype eq "A") {
		 print &dnsLabel . "A record query - $qname\n";
		 my ($ttl, $rdata) = (3600, "192.168.1.1");
		 push @ans, Net::DNS::RR->new("$qname $ttl $qclass $qtype $rdata");
		 $rcode = "NOERROR";
	 }else{
  	          print &dnsLabel . "Something other than an A record lookup:\n";
		  print $query->print . "\n";
	 }
	 
	 # mark the answer as authoritive (by setting the 'aa' flag
	 return ($rcode, \@ans, \@auth, \@add, { aa => 1 });
}
 
# Set the Verbose flag to 1 for more info...
my $ns = Net::DNS::Nameserver->new(
     LocalPort    => 53,
     ReplyHandler => \&reply_handler,
     Verbose      => 0,
) || die "couldn't create nameserver object\n";

print &dnsLabel . "[!] FakedDNS listening...\n";
# Main Loop
$ns->main_loop;

# Make it look pretty
sub dnsLabel {
	print color 'blue';
	print "[DNS] ";
	print color 'reset';
	return "";
}
