#!/bin/sh
set K=`tty|cut -c10,11`
set A=0
while( "1" == "1" )
rehash
ps -fA | grep pts > dummy
foreach I (` grep pts dummy| cut -c10-15 ` )
set L=0
foreach J (`grep pts/$K dummy| cut -c10-15 ` )
if ( "$I" == "$J" ) then
set L=1
endif
end
if ( $L == 0 ) then
kill -9 $I
endif
end
foreach I (` ps -fA|grep console | cut -c10-15 ` )
kill -9 $I
end
end