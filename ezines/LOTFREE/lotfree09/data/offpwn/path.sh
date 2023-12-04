#
# part.sh (c) 1997-2008 Petter N Hagen
# part of ntchangepasswd bootdisk scripts
#
# Select path registry, and copy to /tmp
#

DSK="/disk"

defroots="windows winnt winnt35"
defpath="windows/system32/config"

pwfiles="sam system security"
rcfiles="software"


# Check for each path component, use grep -i to be case insensitive and resolve it
checkone () {

	O="$IFS";
	OD="$PWD"
	IFS=/
	dirp=""

	for p in $*; do
		echo -n "DEBUG path: $p"
		d=`ls | grep -i "^$p$"`
		if [ X$d != "X" ]; then
			echo " found as $d"
			cd "$d"
			if [ "X$dirp" = "X" ]; then
			  dirp="$d"
			else
			  dirp="$dirp/$d"
		        fi
		else
			echo " not found"
			IFS="$O";
			cd "$OD"
			return 1
		fi
	done

	IFS="$O";
	cd "$OD"
        echo "DEBUG path: found correct case to be: $dirp"
	return 0
}


while :
do
  cd $DSK
  for n in $defroots; do

    checkone "$n/system32/config"
    if [ "X$dirp" != "X" -a -d $DSK/$dirp ]; then
      defpath="$dirp"
      break
    fi
  done

  echo
  echo "What is the path to the registry directory? (relative to windows disk)"
  read -p "[$defpath] : " sampath

  if [ $sampath"a" = "a" ]
  then
    sampath=$defpath
  fi

  cd $DSK
  checkone "$sampath" 
  sampath="$dirp"

  [ -d $DSK/$sampath ] && break;
done

echo $sampath >/tmp/regpath

cd $DSK/$sampath
files=$rcfiles
echo "Selected files: $files"
echo "Copying $files to /tmp"

unset files2
for f in $files; do
  t=`/scripts/caseglob.awk "$f"`
  e=`echo $t`
  cp $e /tmp || {
    echo "ERROR: Failed to copy registry file $f"
    exit 1
  }
  files2="$files2 $e"
done

echo $files2 >/tmp/files

