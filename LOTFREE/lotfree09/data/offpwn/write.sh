#
# write.rc (c) 1997-2008 Petter N Hagen
# part of ntchangepasswd bootdisk scripts
#
# Write registry files file back
#
fstype=`cat /tmp/fs`
usepart=`cat /tmp/disk`
sampath=`cat /tmp/regpath`
files=`cat /tmp/files`

for f in $files; do
  echo "Writing $f to $sampath"
  cpnt /tmp/$f /disk/$sampath/$f
done
sync

umount /disk

exit 0

