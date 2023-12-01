#!/bin/sh
## test.sh for Polymorphic Shellcode Engine project
## 
## Made by eee
## 
## Started on  Thu Apr 14 00:41:11 2005 eee
## Last update Thu Apr 14 13:28:43 2005 eee
##

SH="shellcode.c"
OP="shellcode"

echo ""
echo "Hello dude, welcome to my (so er0tic) test script for PSE"
echo ""
echo "The shellcode is in data segment." 
echo "If you have non-executable user pages protection disable it for the test."
echo ""
rm -f $SH $OP
cp ../$SH .
if [ $? -eq 0 ]
    then
    cat >>./$SH<<EOF
int     main()
{
  void  (*execme)() = (void *) shellcode;

  execme();
}
EOF
    echo "Compiling :"
    gcc $SH -o $OP 
    if [ $? -eq 0 ]
	then
	echo ""
	echo "Ok, running the shellcode ->"
	echo ""
	./$OP
    fi
fi
