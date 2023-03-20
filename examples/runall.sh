#!/bin/bash

list=`ls -d */ | sed  's/\///g'`
failedlist=""

for next in $list
do
  echo $next
  if [ -f ${next}/${next}.pro ]; then
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
  else
    echo ${next} not a qt project
  fi
done

echo Failed: ${failedlist}
