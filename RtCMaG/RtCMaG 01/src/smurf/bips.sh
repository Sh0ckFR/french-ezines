#!/bin/bash
# find broadcast ip's that reply with 30+ dupes.

# i decided to make this script into two sections. when running this, make
# sure both parts are in the same directory.

if [ $# != 1 ]; then
echo "$0 <domain - ie: college.edu>"
else
host -l $1 | grep 'has address' | cut -d' ' -f4 > $1.ips
cat $1.ips | cut -d'.' -f1-3 | sort |\
awk '{ print echo ""$1".255" }' > $1.tmp
cat $1.tmp | uniq | awk '{ print "./chekdup.sh "$1"" ]' > $1.ping
rm -f $1.ips $1.tmp
chmod 700 $1.ping
./$1.ping
rm $1.pring
fi
