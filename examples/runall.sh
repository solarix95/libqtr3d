#!/bin/bash

list=`ls -d */ | sed  's/\///g'`
failedlist=""

for next in $list
do
  echo $next
  cd $next
  qmake
  make distclean
  qmake
  make -j15
  ./${next}
  if [ $? -ne 0 ]; then
    echo $ṇext FAILED
    failedlist="${failedlist} ${next}"
  fi
  cd ..
done

echo Failed: ${failedlist}
