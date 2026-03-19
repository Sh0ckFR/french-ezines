#!/bin/bash
# this checks possible broadcast ip's for a given amount of icmp echo
# replies

ping -c 2 $1 > $1.out
if
cat $1.out | grep dupl > /dev/null
then
export DUPES="`cat $1.out | grep dupl | cut -d'+' -f2 | cut -d' ' -f4`"
else
export DUPES=1
fi
if [ $DUPES -gt 30 ]; then
echo "$1 has $DUPES dupes" >> bips.results
rm -f $1.out
else
rm -f $1.out
fi
